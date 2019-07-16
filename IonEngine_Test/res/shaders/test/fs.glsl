#version 420

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
	vec4 lightDir;
	float ambient;
};

in vec3 normal;

out vec4 fragColor;

void main() {
	vec3 light = normalize(lightDir.xyz);
	vec4 diffuse = vec4(color.rgb * max(ambient, dot(normal, light)), color.a);
	fragColor = diffuse;
}
