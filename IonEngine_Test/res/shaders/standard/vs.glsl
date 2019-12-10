#version 420
#pragma pass opaque

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;

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
	float metallic;
	float roughness;
	float reflectance;
};

out vec3 normal;
out vec2 uv;
out mat3x3 tbnMatrix;

void main() {
	uv = vertexUV;

	vec4 worldPos = modelMatrix * vec4(vertexPosition, 1.0);
	normal = normalize((modelMatrix * vec4(vertexNormal, 0.0)).xyz);
	
	vec3 t = normalize(vec3(modelMatrix * vec4(vertexTangent, 0.0)));
	vec3 b = normalize(vec3(modelMatrix * vec4(vertexBitangent, 0.0)));
	vec3 n = normalize(vec3(modelMatrix * vec4(vertexNormal, 0.0)));
	tbnMatrix = mat3x3(t, b, n);

	gl_Position = projectionMatrix * viewMatrix * worldPos;
}
