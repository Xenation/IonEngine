#version 420

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

void main() {
	gl_FragDepth = gl_FragCoord.z;
}
