#version 420

layout (std140, binding = 1) uniform GlobalsVars {
	float time;
	vec4 mainDirLightColor;
	vec4 mainDirLightDirection;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
	float test;
};

in vec2 uv;
in vec3 particleVel;
in vec2 particleParams;

out vec4 fragColor;

void main() {
	fragColor = color;
	fragColor.a *= 1.0 - (time - particleParams.x) / particleParams.y;
}
