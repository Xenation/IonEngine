#version 430

#define PI 3.14159265359
#define HALF_PI 1.570796326795

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

layout (std430, binding = 4) buffer LightClusters {
	//uint clusters[48*24*64]; //Total:73728, [32b]:offset
	//uint indexList[48*24*64*2]; //Total:147456, [2b]:type, [10b]:lightId, [20b]:nextPtr + 1 (0 is for end)
	uint clusters[48*24*64*16]; //Total:1179648, [4b]:type, [28b]:lightId (first uint for a cluster is the light count for the cluster)
};

struct PointLight {
	vec4 positionRange;
	vec4 colorIntensity;
};

layout (std430, binding = 5) readonly restrict buffer PointLights {
	uint pointLightCount;
	PointLight pointLights[];
};

struct SpotLight {
	vec4 positionRange;
	vec4 colorIntensity;
	vec4 dirAngle;
};

layout (std430, binding = 6) readonly restrict buffer SpotLights {
	uint spotLightCount;
	SpotLight spotLights[];
};

layout (binding = 0) uniform atomic_uint currentListIndex;

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
}

float sliceNearZ(uint slice) {
	return exp((slice + (64 * log(zNear)) / log(zFar / zNear)) / (64 / log(zFar / zNear)));
}

uvec2 tileCoords(vec2 fCoords) {
	return uvec2(fCoords.x * 48, fCoords.y * 24);
}

void addLightToCluster(uvec3 clusterCoords, uint lightType, uint lightId) {
	uint clusterIndex = (clusterCoords.z * 1152 + clusterCoords.y * 48 + clusterCoords.x) * 16;
	
	uint indexValue = ((lightType << 28) & 0xf0000000) | lightId & 0x0fffffff;
	uint count = clusters[clusterIndex];
	if (count >= 15) return; // Ignore light when list is full
	clusters[clusterIndex + count + 1] = indexValue;
	clusters[clusterIndex] = count + 1;
}

vec3 decodeNormal(vec2 v) {
	vec3 n = vec3(v.x, v.y, sqrt(1 - v.x * v.x - v.y * v.y));
	uint negFlag = floatBitsToUint(n.x) & 0x00000001;
	if (negFlag == 0) {
		n.z = -n.z;
	}
	return n;
}

vec4 projectionToView(vec4 p) {
	p = invProjectionMatrix * p;
	return p / p.w;
}

float signedDistanceFromPlane(vec4 p, vec4 plane) {
	return dot(plane.xyz, p.xyz);
}

bool intersectFroxelSphere(vec4 nearMinMax, vec4 center, vec2 minMaxZ, float radius) {
	float sqrRadius = radius * radius;

	if (center.z + radius < minMaxZ.x || center.z - radius > minMaxZ.y) return false;
	vec4 projectedCenter = center;
	projectedCenter.z = clamp(projectedCenter.z, minMaxZ.x, minMaxZ.y);
	float sliceScale = projectedCenter.z / minMaxZ.x;
	vec4 slicedMinMax = nearMinMax * sliceScale;
	projectedCenter.xy = clamp(projectedCenter.xy, slicedMinMax.xy, slicedMinMax.zw);

	vec3 projToCenter = center.xyz - projectedCenter.xyz;
	return dot(projToCenter, projToCenter) < sqrRadius;
}

void main() {
	uvec3 clusterPos = gl_WorkGroupID;

	// Compute froxel dimensions
	vec2 sliceMinMax = vec2(sliceNearZ(clusterPos.z), sliceNearZ(clusterPos.z + 1));
	vec2 sliceMinMaxProjection = (sliceMinMax * projectionMatrix[2][2] + projectionMatrix[3][2]) / sliceMinMax;
	vec2 tileMinMaxX = vec2((clusterPos.x / 48.0f - 0.5f) * 2, ((clusterPos.x + 1) / 48.0f - 0.5f) * 2);
	vec2 tileMinMaxY = vec2((clusterPos.y / 24.0f - 0.5f) * 2, ((clusterPos.y + 1) / 24.0f - 0.5f) * 2);

	// Compute froxel near plane rectangle
	vec4 nearMinMax = vec4(projectionToView(vec4(tileMinMaxX.x, tileMinMaxY.x, sliceMinMaxProjection.x, 1)).xy, projectionToView(vec4(tileMinMaxX.y, tileMinMaxY.y, sliceMinMaxProjection.x, 1)).xy);

	// Test PointLights
	for (int li = 0; li < pointLightCount; li++) {
		float range = pointLights[li].positionRange.w;
		vec4 center = viewMatrix * vec4(pointLights[li].positionRange.xyz, 1.0);
		if (intersectFroxelSphere(nearMinMax, center, sliceMinMax, range)) {
			addLightToCluster(clusterPos, 1, li);
		}
	}

	// Test SpotLights
	for (int li = 0; li < spotLightCount; li++) {
		float range = spotLights[li].positionRange.w;
		float angle = spotLights[li].dirAngle.w;
		vec4 center = viewMatrix * vec4(spotLights[li].positionRange.xyz, 1.0);
		if (/*angle > PI && */intersectFroxelSphere(nearMinMax, center, sliceMinMax, range)) { // TODO have a tighter sphere when possible (angles lower than PI)
			addLightToCluster(clusterPos, 2, li);
		}
	}

}
