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

out vec4 fragColor;

void main() {
	vec4 diffuse = color;
	float dirLight = clamp(dot(normal, mainDirLightDirection.xyz), 0, 1);
	diffuse.rgb *= mainDirLightColor.rgb * mainDirLightColor.a * dirLight;
	diffuse.rgb = max(vec3(ambient), diffuse.rgb);
	fragColor = diffuse;
}
