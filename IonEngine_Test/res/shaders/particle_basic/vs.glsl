#version 420
#pragma pass transparent

layout (location = 0) in vec3 particlePos;
layout (location = 1) in vec3 particleVel;
layout (location = 2) in vec2 particleParams;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
	uint sampleCount;
};

uniform mat4x4 modelMatrix;

layout (std140, binding = 10) uniform Material {
	vec4 color;
	float test;
};

out vec3 geomPartVel;
out vec2 geomPartParams;

void main() {
	geomPartVel = particleVel;
	geomPartParams = particleParams;
	gl_Position = vec4(particlePos, 1.0);
}
