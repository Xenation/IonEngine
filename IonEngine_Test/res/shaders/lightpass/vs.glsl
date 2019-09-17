#version 430

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexUV;

out vec2 uv;

void main() {
	uv = vertexUV;
	gl_Position = vec4(vertexPos, 1.0);
}
