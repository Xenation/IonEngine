#version 430

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexUV;

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

out vec2 uv;

void main() {
	uv = vertexUV;
	gl_Position = vec4(vertexPos, 1.0);
}
