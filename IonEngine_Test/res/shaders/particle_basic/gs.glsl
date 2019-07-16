#version 420 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
	uint sampleCount;
};

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
	float test;
};

in vec3 geomPartVel[];
in vec2 geomPartParams[];

out vec2 uv;
out vec3 particleVel;
out vec2 particleParams;

void main() {
	if (time - geomPartParams[0].x > geomPartParams[0].y) return;
	
	particleVel = geomPartVel[0];
	particleParams = geomPartParams[0];

	vec4 center = viewMatrix * gl_in[0].gl_Position;

	gl_Position = projectionMatrix * (center + vec4(-0.5, -0.5, 0, 0));
	uv = vec2(0, 0);
	EmitVertex();

	gl_Position = projectionMatrix * (center + vec4(0.5, -0.5, 0, 0));
	uv = vec2(1, 0);
	EmitVertex();

	gl_Position = projectionMatrix * (center + vec4(-0.5, 0.5, 0, 0));
	uv = vec2(0, 1);
	EmitVertex();

	gl_Position = projectionMatrix * (center + vec4(0.5, 0.5, 0, 0));
	uv = vec2(1, 1);
	EmitVertex();

	EndPrimitive();

}
