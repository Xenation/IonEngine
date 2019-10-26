#version 430

layout (std140, binding = 1) uniform GlobalsVars {
	float time;
	vec4 mainDirLightColor;
	vec4 mainDirLightDirection;
};

layout (std140, binding = 2) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 invProjectionMatrix;
	mat4x4 viewMatrix;
	mat4x4 invViewMatrix;
	float zNear;
	float zFar;
	ivec2 resolution;
	uint sampleCount;
};

layout (binding = 0) uniform sampler2DMS gAlbedo;
layout (binding = 1) uniform sampler2DMS gNormal;
layout (binding = 2) uniform sampler2DMS gDepth;
layout (binding = 3) uniform sampler2D shadowAtlas;
layout (binding = 4) uniform sampler2D clusterDebug;

layout (std140, binding = 10) uniform Material {
	uint directionalCount;
	vec4 directionalColors[4];
	vec4 directionalParams[4];
	uint pointCount;
	vec4 pointColors[32];
	vec4 pointParams[32];
	uint spotCount;
	vec4 spotColors[32];
	vec4 spotPos[32];
	vec4 spotParams[32];
	ivec4 shadowAtlasIndices[32];
	vec4 shadowAtlasCoords[34];
	mat4x4 shadowAtlasWTLMatrices[34];
};

layout (std430, binding = 4) buffer LightClusters {
	uint clusters[48*24*64*16]; //Total:1179648, [4b]:type, [28b]:lightId (first uint for a cluster is the light count for the cluster)
};

in vec2 uv;

out vec4 fragColor;

float remap(float val, float inMin, float inMax, float outMin, float outMax) {
	return (val - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

vec3 decodeNormal(vec2 v) {
	vec3 n = vec3(v.x, v.y, sqrt(1 - v.x * v.x - v.y * v.y));
	uint negFlag = floatBitsToUint(n.x) & 0x00000001;
	if (negFlag == 0) {
		n.z = -n.z;
	}
	return n;
}

vec4 viewPosFromDepth(vec2 uv, float depth, mat4 invProj) {
	vec4 viewPos = invProj * vec4((uv * 2 - 1), depth, 1.0);
	viewPos /= viewPos.w;
	return viewPos;
}
vec4 worldPosFromView(vec4 viewPos, mat4 invView) {
	vec4 worldPos = invView * viewPos;
	return worldPos;
}
vec4 worldPosFromDepth(vec2 uv, float depth, mat4 invView, mat4 invProj) {
	vec4 worldPos = invProj * vec4((uv * 2 - 1), depth, 1.0);
	worldPos /= worldPos.w;
	worldPos = invView * worldPos;
	return worldPos;
}

float sampleShadowAtlas(vec4 viewport, vec2 pos) {
	if (pos.x < 0 || pos.x > 1 || pos.y < 0 || pos.y > 1) return 1.0;
	vec2 atlasPos = viewport.xy + pos * viewport.zw;
	return texture(shadowAtlas, atlasPos).r;
}

const float shadowBias = 0.001;
float computeShadow(uint atlasIndex, vec4 worldPos) {
	vec2 texelSize = 1.0 / textureSize(shadowAtlas, 0);

	vec4 lightPos = shadowAtlasWTLMatrices[atlasIndex] * worldPos;
	vec3 projPos = lightPos.xyz / lightPos.w;
	projPos = projPos * 0.5 + 0.5;
	if (projPos.x < 0 || projPos.y < 0 || projPos.z < 0 || projPos.x > 1 || projPos.y > 1 || projPos.z > 1) return 1.0;
	float currentDepth = projPos.z;

	float shadow = 0;
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			float lightDepth = sampleShadowAtlas(shadowAtlasCoords[atlasIndex], projPos.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth > lightDepth) ? 0.0 : 1.0;
		}
	}
	shadow /= 9;

	return shadow;
}

float computeShadowHard(uint atlasIndex, vec4 worldPos) {
	vec4 lightPos = shadowAtlasWTLMatrices[atlasIndex] * worldPos;
	vec3 projPos = lightPos.xyz / lightPos.w;
	projPos = projPos * 0.5 + 0.5;
	if (projPos.x < 0 || projPos.y < 0 || projPos.z < 0 || projPos.x > 1 || projPos.y > 1 || projPos.z > 1) return 1.0;
	float currentDepth = projPos.z;

	float lightDepth = sampleShadowAtlas(shadowAtlasCoords[atlasIndex], projPos.xy).r;

	return (currentDepth > lightDepth) ? 0.0 : 1.0;
}

uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
}

uvec2 tileCoords(vec2 fCoords) {
	return uvec2(fCoords.x * 48, fCoords.y * 24);
}

uint getClusterIndex(uvec3 clusterCoords) {
	return (clusterCoords.z * 1152 + clusterCoords.y * 48 + clusterCoords.x) * 16;
}

vec3 shadePointLight(vec4 worldPos, vec3 normal, vec4 albedo, uint pointLightId) {
	vec3 lightDir = pointParams[pointLightId].xyz - worldPos.xyz;
	float distSqr = dot(lightDir, lightDir);
	float rangeSqr = pointParams[pointLightId].w * pointParams[pointLightId].w;
	if (distSqr > rangeSqr) return vec3(0);
	vec4 lightColor = vec4(0, 0, 0, 1 - clamp(distSqr / rangeSqr, 0, 1));
	lightDir /= sqrt(distSqr);
	lightColor = pointColors[pointLightId] * lightColor.a;
	return max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a;
}

vec3 shadeSpotLight(vec4 worldPos, vec3 normal, vec4 albedo, uint spotLightId) {
	vec3 lightDir = spotPos[spotLightId].xyz - worldPos.xyz;
	float distSqr = dot(lightDir, lightDir);
	float rangeSqr = spotPos[spotLightId].w * spotPos[spotLightId].w;
	if (distSqr > rangeSqr) return vec3(0);
	lightDir /= sqrt(distSqr);
	vec3 spotDir = decodeNormal(spotParams[spotLightId].xy);
	float atten = clamp(remap(acos(dot(spotDir, -lightDir)), spotParams[spotLightId].w * 0.5, spotParams[spotLightId].z * 0.5, 0, 1), 0, 1);
	if (atten < 0) return vec3(0);
	vec4 lightColor = vec4(0, 0, 0, (1 - clamp(distSqr / rangeSqr, 0, 1)) * atten);
	lightColor = spotColors[spotLightId] * lightColor.a;
	float shadowMult;
	if (shadowAtlasIndices[spotLightId].z >= 0) {
		shadowMult = computeShadow(shadowAtlasIndices[spotLightId].z, worldPos);
	} else {
		shadowMult = 1.0;
	}
	return max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a * shadowMult;
}

void main() {
	ivec2 pixel = ivec2(uv * resolution);

	uvec2 tCoords = tileCoords(uv);

	// TODO take inverted matrices as input
	mat4 invViewMatrix = inverse(viewMatrix);
	mat4 invProjMatrix = inverse(projectionMatrix);

	vec3 fColor = vec3(0);
	for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
		vec4 albedo = texelFetch(gAlbedo, pixel, sampleIndex);
		vec3 normal = texelFetch(gNormal, pixel, sampleIndex).rgb * 2.0 - 1.0;
		float depth = texelFetch(gDepth, pixel, sampleIndex).x * 2.0 - 1.0;

		vec4 worldPos = worldPosFromDepth(uv, depth, invViewMatrix, invProjMatrix);

		vec4 viewPos = viewPosFromDepth(uv, depth, invProjMatrix);
		uint clusterIndex = getClusterIndex(uvec3(tileCoords(uv), depthSlice(viewPos.z)));

		if (albedo.w < 0.5) {
			fColor += albedo.rgb;
			continue;
		}

		vec3 sColor = albedo.rgb * 0.1;

		float shadowMult;
		for (uint i = 0; i < directionalCount; i++) {
			if (shadowAtlasIndices[i].x >= 0) {
				shadowMult = computeShadow(shadowAtlasIndices[i].x, worldPos);
			} else {
				shadowMult = 1.0;
			}
			vec3 lightDir = -directionalParams[i].xyz;
			vec4 lightColor = directionalColors[i];
			sColor += max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a * shadowMult;
		}

		uint counted = 0;
		uint clusterLightCount = clusters[clusterIndex];
		for (uint i = clusterIndex + 1; i < clusterIndex + clusterLightCount + 1; i++) {
			counted++;
			
			uint lightType = (clusters[i] >> 28) & 0x0000000f;
			uint lightId = clusters[i] & 0x0fffffff;

			if (lightType == 1) {
				sColor += shadePointLight(worldPos, normal, albedo, lightId);
			} else if (lightType == 2) {
				sColor += shadeSpotLight(worldPos, normal, albedo, lightId);
			}
		}

		//fragColor = vec4(float(counted) / 2, max(float(counted) - 2, 0), 0, 1);
		//return;

		fColor += sColor + vec3(float(counted) / 2, max(float(counted) - 2, 0), 0);
	}
	fColor /= sampleCount;

	fragColor = vec4(fColor, 1.0);
}
