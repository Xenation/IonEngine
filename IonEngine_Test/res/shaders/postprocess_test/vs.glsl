#version 420
#pragma pass postprocess

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexUV;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
	uint sampleCount;
};

out vec2 uv;

void main() {
	uv = vertexUV;
	gl_Position = vec4(vertexPos, 1.0);
}
