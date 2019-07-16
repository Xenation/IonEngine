#version 420

in vec2 uv;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
	uint sampleCount;
};

layout (binding = 0) uniform sampler2DMS mainTex;
layout (binding = 1) uniform sampler2DMS depthTex;

layout (std140, binding = 10) uniform Material {
	float blend;
};

out vec4 fragColor;

vec4 textureMS(sampler2DMS sampl, ivec2 pixel) {
	vec4 col = vec4(0);
	for (int i = 0; i < sampleCount; i++) {
		col += texelFetch(sampl, pixel, i);
	}
	col /= sampleCount;
	return col;
}

void main() {
	//vec2 pixelUV = 1.0 / resolution;
	ivec2 pixelCoord = ivec2(uv * resolution);
	vec4 col = textureMS(mainTex, pixelCoord);

	vec4 depth = textureMS(depthTex, pixelCoord);
	vec4 depthUp = textureMS(depthTex, pixelCoord + ivec2(0, 1));
	vec4 depthDown = textureMS(depthTex, pixelCoord + ivec2(0, -1));
	vec4 depthRight = textureMS(depthTex, pixelCoord + ivec2(1, 0));
	vec4 depthLeft = textureMS(depthTex, pixelCoord + ivec2(-1, 0));

	float edge = (depthUp.x - depth.x) + (depthDown.x - depth.x) + (depthRight.x - depth.x) + (depthLeft.x - depth.x);
	edge *= 1000;
	edge = step(0.001, edge);

	vec4 pcol = col * (1.0 - edge);

	fragColor = col * (1.0 - blend) + pcol * blend;
}
