#version 420

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
	vec4 mainDirLightColor;
	vec4 mainDirLightDirection;
};

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	float zNear;
	float zFar;
	ivec2 resolution;
	uint sampleCount;
};

layout (binding = 0) uniform sampler2DMS gAlbedo;
layout (binding = 1) uniform sampler2DMS gNormal;
layout (binding = 2) uniform sampler2DMS gDepth;

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

void main() {
	ivec2 pixel = ivec2(uv * resolution);

	vec3 fColor = vec3(0);
	for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
		vec4 albedo = texelFetch(gAlbedo, pixel, sampleIndex);
		vec3 normal = texelFetch(gNormal, pixel, sampleIndex).rgb * 2.0 - 1.0;
		float depth = texelFetch(gDepth, pixel, sampleIndex).x * 2.0 - 1.0;

		// TODO take inverted matrices as input
		vec4 worldPos = inverse(projectionMatrix) * vec4((uv * 2 - 1), depth, 1.0);
		worldPos /= worldPos.w;
		worldPos = inverse(viewMatrix) * worldPos;

		if (albedo.w < 0.5) {
			fColor += albedo.rgb;
			continue;
		}

		vec3 sColor = albedo.rgb * 0.1;

		for (uint i = 0; i < directionalCount; i++) {	
			vec3 lightDir = -directionalParams[i].xyz;
			vec4 lightColor = directionalColors[i];
			sColor += max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a;
		}

		for (uint i = 0; i < pointCount; i++) {
			vec3 lightDir = pointParams[i].xyz - worldPos.xyz;
			float dist = length(lightDir);
			if (dist > pointParams[i].w) continue;
			vec4 lightColor = vec4(0, 0, 0, 1 - clamp(dist / pointParams[i].w, 0, 1));
			lightDir /= dist;
			lightColor = pointColors[i] * lightColor.a;
			sColor += max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a;
		}

		for (uint i = 0; i < spotCount; i++) {
			vec3 lightDir = spotPos[i].xyz - worldPos.xyz;
			float dist = length(lightDir);
			if (dist > spotPos[i].w) continue;
			lightDir /= dist;
			vec3 spotDir = decodeNormal(spotParams[i].xy);
			float atten = clamp(remap(acos(dot(spotDir, -lightDir)), spotParams[i].w * 0.5, spotParams[i].z * 0.5, 0, 1), 0, 1);
			if (atten < 0) continue;
			vec4 lightColor = vec4(0, 0, 0, (1 - clamp(dist / spotPos[i].w, 0, 1)) * atten);
			lightColor = spotColors[i] * lightColor.a;
			sColor += max(dot(normal, lightDir), 0.0) * albedo.rgb * lightColor.rgb * lightColor.a;
		}

		fColor += sColor;
	}
	fColor /= sampleCount;

	fragColor = vec4(fColor, 1.0);
}
