#version 430

layout (std140, binding = 1) uniform GlobalsVars {
	float time;
	vec4 mainDirLightColor;
	vec4 mainDirLightDirection;
};

layout (std140, binding = 2) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	float zNear;
	float zFar;
	ivec2 resolution;
	uint sampleCount;
};

layout (std430, binding = 4) buffer LightClusters {
	uint clusters[48*24*64]; //Total:73728, [32b]:offset
	uint indexList[48*24*64*2]; //Total:147456, [2b]:type, [10b]:lightId, [20b]:nextPtr + 1 (0 is for end)
};

struct PointLight {
	vec4 positionRange;
	vec4 colorIntensity;
};

layout (std430, binding = 5) readonly restrict buffer PointLights {
	uint pointLightCount;
	PointLight pointLights[32];
};

struct SpotLight {
	vec4 positionRange;
	vec4 colorIntensity;
	vec4 dirAngle;
};

layout (std430, binding = 6) readonly restrict buffer SpotLights {
	uint spotLightCount;
	SpotLight spotLights[32];
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
	// Find the cluster
	uint clusterIndex = clusterCoords.z * 1152 + clusterCoords.y * 48 + clusterCoords.x;
	uint clusterPointer = clusters[clusterIndex];
	uint currentNodeIndex = clusterPointer;

	// Search through the list to avoid adding twice the same light
	while (currentNodeIndex != 0) {
		uint existingLightType = (indexList[currentNodeIndex - 1] >> 30) & 0x00000003;
		uint existingLightId = (indexList[currentNodeIndex - 1] >> 20) & 0x000003ff;
		if (existingLightType == lightType && existingLightId == lightId) { // Avoid adding twice the same light
			return;
		}
		currentNodeIndex = indexList[currentNodeIndex - 1] & 0x000fffff;
	}

	// Allocate a new node
	uint indexValue = ((lightType << 30) & 0xc0000000) | ((lightId << 20) & 0x3ff00000) | clusters[clusterIndex] & 0x000fffff;
	clusters[clusterIndex] = atomicCounterIncrement(currentListIndex);
	indexList[clusters[clusterIndex]] = indexValue;
}

bool clusterHasLight(uvec3 clusterPos, uint lightType, uint lightId) {
	// Find the cluster
	uint clusterIndex = clusterPos.z * 1152 + clusterPos.y * 48 + clusterPos.x;
	uint currentNodeIndex = clusters[clusterIndex];

	// Search the cluster list
	while (currentNodeIndex != 0) {
		uint existingLightType = (indexList[currentNodeIndex - 1] >> 30) & 0x00000003;
		uint existingLightId = (indexList[currentNodeIndex - 1] >> 20) & 0x000003ff;
		if (existingLightType == lightType && existingLightId == lightId) { // Same light
			return true;
		}
		currentNodeIndex = indexList[currentNodeIndex - 1] & 0x000fffff;
	}

	return false;
}

void main() {
	
	uvec3 clusterPos = gl_WorkGroupID;

	vec2 sliceMinMax = vec2(sliceNearZ(clusterPos.z), sliceNearZ(clusterPos.z + 1));
	vec3 corners[8] = {
		vec3(0, 0, sliceMinMax.x),
		vec3(0, 0, sliceMinMax.x),
		vec3(0, 0, sliceMinMax.x),
		vec3(0, 0, sliceMinMax.x),
		vec3(0, 0, sliceMinMax.y),
		vec3(0, 0, sliceMinMax.y),
		vec3(0, 0, sliceMinMax.y),
		vec3(0, 0, sliceMinMax.y)
	};

}
