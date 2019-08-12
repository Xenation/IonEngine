#version 420

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	float zNear;
	float zFar;
	ivec2 resolution;
	uint sampleCount;
};

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
	vec4 mainDirLightColor;
	vec4 mainDirLightDirection;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
	float ambient;
};

in vec3 normal;

layout (location = 0) out vec4 gDiffuse;
layout (location = 1) out vec4 gNormal;

void main() {
	vec4 diffuse = color;
	//float dirLight = max(ambient, clamp(dot(normal, -mainDirLightDirection.xyz), 0, 1));
	//diffuse.rgb *= mainDirLightColor.rgb * mainDirLightColor.a * dirLight;
	gDiffuse = diffuse;
	gNormal.rgb = normal * 0.5 + 0.5;
}
