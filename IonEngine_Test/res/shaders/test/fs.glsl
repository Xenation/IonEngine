#version 420

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
in vec3 wPos;

layout (location = 0) out vec4 gDiffuse;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gWorldPos;

void main() {
	vec4 diffuse = color;
	//float dirLight = max(ambient, clamp(dot(normal, -mainDirLightDirection.xyz), 0, 1));
	//diffuse.rgb *= mainDirLightColor.rgb * mainDirLightColor.a * dirLight;
	gDiffuse = diffuse;
	gWorldPos = vec4(wPos, 1);
	gNormal.rgb = normal * 0.5 + 0.5;
}
