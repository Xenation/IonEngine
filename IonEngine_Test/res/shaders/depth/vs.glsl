#version 420

layout (location = 0) in vec3 vertexPos;

uniform mat4x4 worldToLightMatrix;
uniform mat4x4 modelMatrix;

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

void main() {
	gl_Position = worldToLightMatrix * modelMatrix * vec4(vertexPos, 1.0);
}
