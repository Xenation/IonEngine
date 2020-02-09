#include "Mesh.h"

using namespace IonEngine;



Mesh::Mesh() {
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

Mesh::~Mesh() {

}


bool Mesh::initialize(ID3D11Device* device) {
	bool success = initializeBuffers(device);
	if (!success) {
		return false;
	}

	return true;
}

void Mesh::shutdown() {
	shutdownBuffers();
}

void Mesh::render(ID3D11DeviceContext* deviceContext) {
	renderBuffers(deviceContext);
}

i32 Mesh::getIndexCount() {
	return indexCount;
}

bool Mesh::initializeBuffers(ID3D11Device* device) {
	Vertex* vertices;
	u32* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexCount = 3;
	indexCount = 3;

	// Create the vertex array
	vertices = new Vertex[vertexCount];
	if (vertices == nullptr) {
		return false;
	}

	// Create indices array
	indices = new u32[indexCount];
	if (indices == nullptr) {
		return false;
	}

	// Load the vertex array with data
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	 // Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// Setup the desc of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(u32) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Release the arrays
	delete[] vertices;
	vertices = nullptr;
	delete[] indices;
	indices = nullptr;

	return true;
}

void Mesh::shutdownBuffers() {
	if (indexBuffer != nullptr) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer != nullptr) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void Mesh::renderBuffers(ID3D11DeviceContext* deviceContext) {
	u32 stride = sizeof(Vertex);
	u32 offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
