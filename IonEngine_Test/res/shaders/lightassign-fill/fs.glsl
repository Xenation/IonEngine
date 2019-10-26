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

layout (std430, binding = 4) buffer LightClusters {
	uint clusters[48*24*64]; //Total:73728, [32b]:offset
	uint indexList[48*24*64*2]; //Total:147456, [2b]:type, [10b]:lightId, [20b]:nextPtr + 1 (0 is for end)
};

layout (binding = 0) uniform atomic_uint currentListIndex;

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

uniform uint lightType;
uniform uint lightId;

layout (binding = 0) uniform sampler2D minMaxTex;

in vec2 uv;

out float frag;


uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
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

void writeClusters(uvec2 tile, uint minSlice, uint maxSlice) {
	for (uint z = minSlice; z <= maxSlice; z++) {
		addLightToCluster(uvec3(tile, z), lightType, lightId);
	}
}

void main() {
	vec2 minMax = texture2D(minMaxTex, uv).xy;

	if (minMax.x >= 0.0 && minMax.y >= 0.0) {
		writeClusters(uvec2(gl_FragCoord.xy), uint(minMax.x), uint(minMax.y));
		frag = 1.0;
	} else {
		frag = -1.0;
	}
}
