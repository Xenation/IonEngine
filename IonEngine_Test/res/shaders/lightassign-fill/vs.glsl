#version 430

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexUV;

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

out vec2 uv;


void main() {
	uv = vertexUV;
	gl_Position = vec4(vertexPos, 1.0);
}
