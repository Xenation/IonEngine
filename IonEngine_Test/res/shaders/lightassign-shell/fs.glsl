#version 430

#define PI 3.1415926535897932384626433832795

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

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

//uniform uint lightType;
//uniform uint lightId;
uniform bool useMin;

flat in uvec2 triMinMaxSlices;
flat in vec3 triangle[3];

out vec2 frag;


uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
}

uvec2 tileCoords(vec2 fCoords) {
	return uvec2(fCoords.x * 48, fCoords.y * 24);
}

vec3 barycentricCoords(vec3 v0, vec3 v1, vec3 v2, vec2 p) {
	float divider = (v2.y - v1.y) * (v0.x - v1.x) + (v1.x - v2.x) * (v0.y - v1.y);
	//if (divider == 0) return false;
	float weightV0 = ((v2.y - v1.y) * (p.x - v1.x) + (v1.x - v2.x) * (p.y - v1.y)) / divider;
	float weightV2 = ((v1.y - v0.y) * (p.x - v1.x) + (v0.x - v1.x) * (p.y - v1.y)) / divider;
	float weightV1 = 1.0f - weightV0 - weightV2;
	return vec3(weightV0, weightV1, weightV2);
}

bool intersectSegmentPlane(vec3 p0, vec3 p1, vec3 pn, out float lerpVal) {
	vec3 u = p1 - p0;

	float d = dot(pn, u);
	float n = -dot(pn, p0);

	lerpVal = n / d;
	return !(lerpVal != clamp(lerpVal, 0, 1)); // TODO that clamp handles NaN and INF as expected
}

bool isInXSlice(vec3 topPlane, vec3 bottomPlane, vec3 v) {
	return topPlane.y * v.y + topPlane.z * v.z >= 0.0 && bottomPlane.y * v.y + bottomPlane.z * v.z >= 0.0;
}

bool isInYSlice(vec3 leftPlane, vec3 rightPlane, vec3 v) {
	return leftPlane.x * v.x + leftPlane.z * v.z >= 0.0 && rightPlane.x * v.x + rightPlane.z * v.z >= 0.0;
}

bool intersectRayTriangle(vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, out float z) {
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;
	vec3 q = cross(rayDir, e2);
	float a = dot(e1, q);

	if (a > -0.000001f && a < 0.000001f) {
		return false;
	}

	float f = 1.0f / a;
	float u = f * dot(-v0, q);

	if (u != clamp(u, 0, 1)) {
		return false;
	}

	vec3 r = cross(-v0, e1);
	float v = f * dot(rayDir, r);

	if (v < 0.0f || (u + v) > 1.0f) {
		return false;
	}

	z = f * dot(e2, r) * rayDir.z;

	return true;
}

vec2 minMax(vec2 depthMinMax, in float depth) {
	depthMinMax.x = min(depthMinMax.x, depth);
	depthMinMax.y = max(depthMinMax.y, depth);

	return depthMinMax;
}

void main() {
	
	// DUMB TRIANGLE DEPTH RANGE METHOD
	frag = triMinMaxSlices;

	return;



	// RAYCAST METHOD
	vec2 depthMinMax = vec2(1000, 0.0f);
	const float ey = tan(PI / 4.0 * 0.5);
	const float ex = ey * resolution.x / resolution.y;

	const vec3 leftPlane = normalize(vec3(1.0, 0.0, (1.0 - 2.0 * floor(gl_FragCoord.x) / 48) * ex));
	const vec3 rightPlane = -normalize(vec3(1.0, 0.0, (1.0 - 2.0 * floor(gl_FragCoord.x + 1.0) / 48) * ex));
	const vec3 topPlane = normalize(vec3(0.0, -1.0, (1.0 - 2.0 * floor(gl_FragCoord.y) / 24) * ey));
	const vec3 bottomPlane = -normalize(vec3(0.0, -1.0, (1.0 - 2.0 * floor(gl_FragCoord.y + 1.0) / 24) * ey));

	// Case 4 corners
	float zPos;
	depthMinMax = intersectRayTriangle(cross(topPlane, leftPlane), triangle[0], triangle[1], triangle[2], zPos) ? minMax(depthMinMax, zPos) : depthMinMax;
	depthMinMax = intersectRayTriangle(cross(topPlane, rightPlane), triangle[0], triangle[1], triangle[2], zPos) ? minMax(depthMinMax, zPos) : depthMinMax;
	depthMinMax = intersectRayTriangle(cross(rightPlane, bottomPlane), triangle[0], triangle[1], triangle[2], zPos) ? minMax(depthMinMax, zPos) : depthMinMax;
	depthMinMax = intersectRayTriangle(cross(bottomPlane, leftPlane), triangle[0], triangle[1], triangle[2], zPos) ? minMax(depthMinMax, zPos) : depthMinMax;

	// Case triangle corners
	depthMinMax = isInXSlice(topPlane, bottomPlane, triangle[0]) && isInYSlice(leftPlane, rightPlane, triangle[0]) ? minMax(depthMinMax, triangle[0].z) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, triangle[1]) && isInYSlice(leftPlane, rightPlane, triangle[1]) ? minMax(depthMinMax, triangle[1].z) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, triangle[2]) && isInYSlice(leftPlane, rightPlane, triangle[2]) ? minMax(depthMinMax, triangle[2].z) : depthMinMax;
	
	// Case edges
	float lerpVal;
	// left side
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[0], triangle[1], leftPlane, lerpVal) ? mix(triangle[0], triangle[1], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[0].z, triangle[1].z, lerpVal)) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[1], triangle[2], leftPlane, lerpVal) ? mix(triangle[1], triangle[2], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[1].z, triangle[2].z, lerpVal)) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[2], triangle[0], leftPlane, lerpVal) ? mix(triangle[2], triangle[0], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[2].z, triangle[0].z, lerpVal)) : depthMinMax;
	// right side
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[0], triangle[1], rightPlane, lerpVal) ? mix(triangle[0], triangle[1], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[0].z, triangle[1].z, lerpVal)) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[1], triangle[2], rightPlane, lerpVal) ? mix(triangle[1], triangle[2], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[1].z, triangle[2].z, lerpVal)) : depthMinMax;
	depthMinMax = isInXSlice(topPlane, bottomPlane, intersectSegmentPlane(triangle[2], triangle[0], rightPlane, lerpVal) ? mix(triangle[2], triangle[0], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[2].z, triangle[0].z, lerpVal)) : depthMinMax;
	// bottom side
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[0], triangle[1], bottomPlane, lerpVal) ? mix(triangle[0], triangle[1], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[0].z, triangle[1].z, lerpVal)) : depthMinMax;
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[1], triangle[2], bottomPlane, lerpVal) ? mix(triangle[1], triangle[2], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[1].z, triangle[2].z, lerpVal)) : depthMinMax;
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[2], triangle[0], bottomPlane, lerpVal) ? mix(triangle[2], triangle[0], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[2].z, triangle[0].z, lerpVal)) : depthMinMax;
	// top side
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[0], triangle[1], topPlane, lerpVal) ? mix(triangle[0], triangle[1], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[0].z, triangle[1].z, lerpVal)) : depthMinMax;
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[1], triangle[2], topPlane, lerpVal) ? mix(triangle[1], triangle[2], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[1].z, triangle[2].z, lerpVal)) : depthMinMax;
	depthMinMax = isInYSlice(leftPlane, rightPlane, intersectSegmentPlane(triangle[2], triangle[0], topPlane, lerpVal) ? mix(triangle[2], triangle[0], lerpVal) : vec3(0, 0, -1)) ? minMax(depthMinMax, mix(triangle[2].z, triangle[0].z, lerpVal)) : depthMinMax;

	frag = depthMinMax;

	return;


	
	// BARYCENTRIC METHOD
	ivec2 res = ivec2(48, 24); // TODO ugly hard coded, should use 'resolution' but isn't updated to current target size
	
	vec3 corners[4] = {
		vec3((vec2(gl_FragCoord.xy + vec2(-0.5, -0.5)) / res - 0.5) * 2.0, 0.0),
		vec3((vec2(gl_FragCoord.xy + vec2(0.5, -0.5)) / res - 0.5) * 2.0, 0.0),
		vec3((vec2(gl_FragCoord.xy + vec2(0.5, 0.5)) / res - 0.5) * 2.0, 0.0),
		vec3((vec2(gl_FragCoord.xy + vec2(-0.5, 0.5)) / res - 0.5) * 2.0, 0.0)
	};

	if (gl_FrontFacing) { // Front Shell
		for (int i = 0; i < 4; i++) {
			vec3 cornerBary = barycentricCoords(triangle[0], triangle[1], triangle[2], corners[i].xy);
			corners[i].z = triangle[0].z * cornerBary.x + triangle[1].z * cornerBary.y + triangle[2].z * cornerBary.z;
		}
		
		uint minSlice = 1000;
		for (int i = 0; i < 4; i++) { // TODO compute slice only once the mindepth has been found
			uint cornerSlice = depthSlice(corners[i].z);
			if (cornerSlice < minSlice) {
				minSlice = cornerSlice;
			}
		}
		minSlice = clamp(minSlice, triMinMaxSlices.x, triMinMaxSlices.y);
		frag = vec2(minSlice, 0);
	} else { // Back Shell
		for (int i = 0; i < 4; i++) {
			vec3 cornerBary = barycentricCoords(triangle[0], triangle[1], triangle[2], corners[i].xy);
			corners[i].z = triangle[0].z * cornerBary.x + triangle[1].z * cornerBary.y + triangle[2].z * cornerBary.z;
		}

		uint maxSlice = 0;
		for (int i = 0; i < 4; i++) {
			uint cornerSlice = depthSlice(corners[i].z);
			if (cornerSlice > maxSlice) {
				maxSlice = cornerSlice;
			}
		}
		maxSlice = clamp(maxSlice, triMinMaxSlices.x, triMinMaxSlices.y);
		frag = vec2(0, maxSlice);
	}
}
