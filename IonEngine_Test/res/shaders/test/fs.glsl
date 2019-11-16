#version 420

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

layout (std140, binding = 10) uniform Material {
	vec4 color;
	float ambient;
	float metallic;
	float roughness;
	float reflectance;
};

in vec3 normal;

layout (location = 0) out vec4 gDiffuse;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gMetallic;

void main() {
	vec4 diffuse = color;
	gDiffuse = diffuse;
	gMetallic.r = metallic;
	gMetallic.g = roughness;
	gMetallic.b = reflectance;
	gNormal.rgb = normal * 0.5 + 0.5;
}
