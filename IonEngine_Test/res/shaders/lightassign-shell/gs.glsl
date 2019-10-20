#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 2) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	float zNear;
	float zFar;
	ivec2 resolution;
	uint sampleCount;
};

in vec4 viewPos[];

flat out uvec2 triMinMaxSlices;
flat out vec3 triangle[3];


uint depthSlice(float viewDepth) {
	return uint(log(viewDepth) * (64 / log(zFar / zNear)) - ((64 * log(zNear)) / log(zFar / zNear)));
}

void main() {
	
	float minDepth = zFar;
	float maxDepth = zNear;
	for (int i = 0; i < 3; i++) {
		if (gl_in[i].gl_Position.w < minDepth) {
			minDepth = gl_in[i].gl_Position.w;
		}
		if (gl_in[i].gl_Position.w > maxDepth) {
			maxDepth = gl_in[i].gl_Position.w;
		}
	}
	triMinMaxSlices.x = depthSlice(minDepth);
	triMinMaxSlices.y = depthSlice(maxDepth);

	triangle[0] = viewPos[0].xyz;
	//triangle[0].xy /= gl_in[0].gl_Position.w;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	triangle[1] = viewPos[1].xyz;
	//triangle[1].xy /= gl_in[1].gl_Position.w;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	triangle[2] = viewPos[2].xyz;
	//triangle[2].xy /= gl_in[2].gl_Position.w;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
