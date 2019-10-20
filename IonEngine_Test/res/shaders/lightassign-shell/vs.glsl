#version 430

layout (location = 0) in vec3 vertexPos;

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

uniform mat4x4 modelMatrix;

out vec4 viewPos;


void main() {
	viewPos = viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
	gl_Position = projectionMatrix * viewPos;
}
