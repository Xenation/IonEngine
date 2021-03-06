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
	float metallic;
	float roughness;
	float reflectance;
};

in vec3 normal;
in vec2 uv;
in mat3x3 tbnMatrix;

layout (location = 0) out vec4 gDiffuse;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gMetallic;

layout (binding = 0) uniform sampler2D diffuseTex;
layout (binding = 1) uniform sampler2D normalTex;
layout (binding = 2) uniform sampler2D mraTex;

vec3 bumpToNormal(sampler2D bTex, vec2 uv) {
	vec4 b = textureOffset(bTex, uv, ivec2(0, 0));
	vec4 bu = textureOffset(bTex, uv, ivec2(0, 1));
	vec4 br = textureOffset(bTex, uv, ivec2(1, 0));

	vec3 n = vec3(1.0);
	n.x = b.r - br.r;
	n.y = b.r - bu.r;

	return normalize(n);
}

void main() {
	vec4 diffuse = texture(diffuseTex, uv);
	vec4 mra = texture(mraTex, uv);
	vec3 normalMap = texture(normalTex, uv).xyz * 2.0 - 1.0;
	normalMap = normalize(tbnMatrix * normalMap);
	gDiffuse = diffuse;
	gMetallic.r = mra.r * metallic;
	gMetallic.g = mra.g * roughness;
	gMetallic.b = reflectance;
	gNormal.rgb = normalMap * 0.5 + 0.5;
}
