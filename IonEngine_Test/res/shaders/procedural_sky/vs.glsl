#version 420
#pragma pass opaque

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

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

uniform mat4x4 modelMatrix;

layout (std140, binding = 10) uniform Material {
	vec4 skyColor;
	vec4 bottomColor;
	float lowerAngle;
	float upperAngle;
};

out vec3 direction;


void main() {
	direction = vertexNormal;
	mat4x4 rotViewMatrix = mat4x4(mat3x3(viewMatrix));
	vec4 clipPos = projectionMatrix * rotViewMatrix * vec4(vertexPosition, 1.0);
	gl_Position = clipPos.xyww;
}
