#version 430

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

layout (std430, binding = 4) buffer LightClusters {
	uint listIndex;
	uint clusters[48*24*64]; //Total:73728, [32b]:offset
	uint indexList[48*24*64*2]; //Total:147456, [2b]:type, [10b]:lightId, [20b]:nextPtr + 1 (0 is for end)
};
layout (binding = 4) uniform atomic_uint currentListIndex;

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

uniform uint lightType;
uniform uint lightId;

flat in uvec2 triMinMaxSlices;

out vec4 frag;


uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
}

uvec2 tileCoords(vec2 fCoords) {
	return uvec2(fCoords.x * 48, fCoords.y * 24);
}

uint addLightIndex(uint lightType, uint lightId, uint prevIndex) {
	// The atomic counter to safely "allocate" a new node
	uint listIndex = atomicCounterIncrement(currentListIndex);
	
	// Assign the new node's index to the next of the previous node (if there is a previous)
	if (prevIndex != 0) {
		indexList[prevIndex - 1] |= 0x000fffff & (listIndex + 1);
	}

	// Build the node
	uint indexValue = (lightType << 30) | (lightId << 20) | 0;
	indexList[listIndex] = indexValue;

	return listIndex;
}

void addLightToCluster(uvec3 clusterCoords, uint lightType, uint lightId) {
	// Find the cluster
	uint clusterIndex = clusterCoords.z * 1152 + clusterCoords.y * 48 + clusterCoords.x;
	uint currentNodeIndex = clusters[clusterIndex];
	uint lastNode = 0;

	// Find the end of the cluster's list
	while (currentNodeIndex != 0) {
		lastNode = currentNodeIndex;
		uint existingLightType = (indexList[currentNodeIndex - 1] >> 30) & 0x00000003;
		uint existingLightId = (indexList[currentNodeIndex - 1] >> 20) & 0x000003ff;
		if (existingLightType == lightType && existingLightId == lightId) { // Avoid adding twice the same light
			return;
		}
		currentNodeIndex = indexList[currentNodeIndex - 1] & 0x000fffff;
	}
	
	// Add the light to the list
	uint addedNodeIndex = addLightIndex(lightType, lightId, lastNode);
	if (lastNode == 0) { // If there was no list, make the cluster point towards the created node
		clusters[clusterIndex] = addedNodeIndex + 1;
	}
}

bool clusterHasLight(uvec3 clusterPos, uint lightType, uint lightId) {
	// Find the cluster
	uint clusterIndex = clusterPos.z * 1152 + clusterPos.y * 48 + clusterPos.x;
	uint currentNodeIndex = clusters[clusterIndex];
	uint lastNode = 0;

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

bool findNearClusterWithLight(uvec3 startClusterPos, uint lightType, uint lightId, inout uint nearClusterSlice) {

	// Loop through the clusters further than the current one
	for (int slice = int(startClusterPos.z) - 1; slice >= 0; slice--) {
		// Check if the light is already added to the cluster
		if (clusterHasLight(uvec3(startClusterPos.xy, uint(slice)), lightType, lightId)) {
			nearClusterSlice = uint(slice);
			return true;
		}
	}

	return false;
}

void writeClusters(uvec2 tile, uint minSlice, uint maxSlice) {
	for (uint z = minSlice; z <= maxSlice; z++) {
		addLightToCluster(uvec3(tile, z), lightType, lightId);
	}
}

void main() {
	uint nearClusterSlice = 0;
	findNearClusterWithLight(uvec3(uvec2(gl_FragCoord.xy), triMinMaxSlices.x), lightType, lightId, nearClusterSlice);
	writeClusters(uvec2(gl_FragCoord.xy), nearClusterSlice, triMinMaxSlices.y);

	frag = vec4(triMinMaxSlices.x, triMinMaxSlices.y, 0, 0);
}
