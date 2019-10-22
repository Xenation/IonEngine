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

vec4 projectionToView(mat4x4 invProjection, vec4 p) {
	p = invProjection * p;
	return p / p.w;
}

void main() {
	mat4x4 inverseProjection = inverse(projectionMatrix); // HEAVY
	
	uvec3 clusterPos = gl_WorkGroupID;

	// Compute corners in projection space
	vec2 sliceMinMax = vec2(sliceNearZ(clusterPos.z), sliceNearZ(clusterPos.z + 1));
	vec2 sliceMinMaxProjection = sliceMinMax * projectionMatrix[2][2] + projectionMatrix[3][2];
	vec2 tileMinMaxX = vec2(clusterPos.x / 48.0f, (clusterPos.x + 1) / 48.0f);
	vec2 tileMinMaxY = vec2(clusterPos.y / 24.0f, (clusterPos.y + 1) / 24.0f);
	vec4 corners[8] = {
		vec4(tileMinMaxX.x, tileMinMaxY.x, sliceMinMaxProjection.x, sliceMinMax.x),
		vec4(tileMinMaxX.x, tileMinMaxY.y, sliceMinMaxProjection.x, sliceMinMax.x),
		vec4(tileMinMaxX.y, tileMinMaxY.y, sliceMinMaxProjection.x, sliceMinMax.x),
		vec4(tileMinMaxX.y, tileMinMaxY.x, sliceMinMaxProjection.x, sliceMinMax.x),
		vec4(tileMinMaxX.x, tileMinMaxY.x, sliceMinMaxProjection.y, sliceMinMax.y),
		vec4(tileMinMaxX.x, tileMinMaxY.y, sliceMinMaxProjection.y, sliceMinMax.y),
		vec4(tileMinMaxX.y, tileMinMaxY.y, sliceMinMaxProjection.y, sliceMinMax.y),
		vec4(tileMinMaxX.y, tileMinMaxY.x, sliceMinMaxProjection.y, sliceMinMax.y)
	};

	// Transform corners to view space
	for (int i = 0; i < 8; i++) {
		corners[i] = projectionToView(inverseProjection, corners[i]);
	}

	// Test PointLights
	for (int li = 0; li < pointLightCount; li++) {
		float sqrRange = pointLights[li].positionRange.w * pointLights[li].positionRange.w;
		for (int ci = 0; ci < 8; ci++) {
			vec3 toLight = (viewMatrix * vec4(pointLights[li].positionRange.xyz, 1)).xyz - corners[ci].xyz;
			float sqrDistance = dot(toLight, toLight);
			if (sqrDistance < sqrRange) {
				addLightToCluster(clusterPos, 1, li);
				break;
			}
		}
	}

}
