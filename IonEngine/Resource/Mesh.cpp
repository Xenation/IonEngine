#include "Mesh.h"

#include "Render/Descriptors.h"
#include "Render/Resource.h"
#include "Render/RenderContext.h"
using namespace IonEngine;



Mesh::Mesh() {
	
}

Mesh::~Mesh() {
	if (state & CPU_ALLOCATED) {
		free();
	}
	if (state & GPU_ALLOCATED) {
		destroy();
	}
}


void Mesh::allocate(u32 vCount, u32 iCount) {
	vertices = new Vertex[vertexCount];
	indices = new u32[indexCount];
	state |= CPU_ALLOCATED;
}

void Mesh::free() {
	delete vertices;
	vertices = nullptr;
	delete indices;
	indices = nullptr;
	state &= ~CPU_ALLOCATED;
}

void Mesh::setVertex(u32 index, const Vertex& vertex) {
	vertices[index] = vertex;
}

void Mesh::setIndex(u32 index, u32 indexValue) {
	indices[index] = indexValue;
}

void Mesh::upload(RenderContext* context) {
	BufferDescriptor vertexDesc = {};
	vertexDesc.usage = ResourceUsage::DEFAULT;
	vertexDesc.size = sizeof(Vertex) * vertexCount;
	vertexDesc.stride = sizeof(Vertex);
	vertexDesc.bindFlags = BindFlags::VERTEX_BUFFER;
	vertexBuffer = context->createBuffer(vertexDesc, vertices);

	BufferDescriptor indexDesc = {};
	indexDesc.usage = ResourceUsage::DEFAULT;
	indexDesc.size = sizeof(u32) * vertexCount;
	indexDesc.stride = sizeof(u32);
	indexDesc.bindFlags = BindFlags::INDEX_BUFFER;
	indexBuffer = context->createBuffer(indexDesc, indices);
}

void Mesh::destroy(RenderContext* context) {
	context->destroyBuffer(vertexBuffer);
	context->destroyBuffer(indexBuffer);
}

void Mesh::render(RenderContext* context) {
	
}
