#version 420
#pragma pass opaque

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

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
	float ambient;
	float metallic;
	float roughness;
	float reflectance;
};

out vec3 normal;
out vec2 uv;

void main() {
	vec4 worldPos = modelMatrix * vec4(vertexPosition, 1.0);
	normal = normalize((modelMatrix * vec4(vertexNormal, 0.0)).xyz);
	uv = vertexUV;
	gl_Position = projectionMatrix * viewMatrix * worldPos;
}
