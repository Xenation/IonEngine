#version 420

#define DEG_TO_RAD 0.0174533

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
	vec4 skyColor;
	vec4 bottomColor;
	float lowerAngle;
	float upperAngle;
};

in vec3 direction;

out vec4 fragColor;


float remap(float val, float inMin, float inMax, float outMin, float outMax) {
	return (val - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

void main() {
	float DoU = dot(direction, vec3(0.0, 1.0, 0.0));
	float s = clamp(remap(acos(DoU), lowerAngle * DEG_TO_RAD, upperAngle * DEG_TO_RAD, 0.0, 1.0), 0.0, 1.0);
	fragColor = mix(bottomColor, skyColor, s);
}
