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

void Mesh::setRenderContext(RenderContext* context) {
	this->context = context;
}

void Mesh::upload() {
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

	state |= GPU_ALLOCATED;
}

void Mesh::destroy() {
	context->destroyBuffer(vertexBuffer);
	context->destroyBuffer(indexBuffer);

	state &= ~GPU_ALLOCATED;
}

void Mesh::render() {
#if ION_DX11
	u32 stride = sizeof(Vertex);
	u32 offset = 0;
	
	ID3D11DeviceContext* apiDeviceContext = context->getDeviceContext();
	ID3D11Buffer* apiVertexBuffer = vertexBuffer->getAPIResource();
	ID3D11Buffer* apiIndexBuffer = indexBuffer->getAPIResource();

	apiDeviceContext->IASetVertexBuffers(0, 1, &apiVertexBuffer, &stride, &offset);
	apiDeviceContext->IASetIndexBuffer(apiIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	apiDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif
}
