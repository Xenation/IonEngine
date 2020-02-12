#include "Buffer.h"

#include "RenderContext.h"
#include "ResourceUtils.h"
using namespace IonEngine;



Buffer::Buffer(const BufferDescriptor desc) : descriptor(desc) {
	apiDesc.Usage = convertResourceUsage(descriptor.usage);
	apiDesc.ByteWidth = descriptor.size;
	apiDesc.BindFlags = convertBindFlags(descriptor.bindFlags);
	apiDesc.CPUAccessFlags = 0;
	apiDesc.MiscFlags = 0;
	apiDesc.StructureByteStride = 0; // descriptor.stride?
}


Buffer* RenderContext::createBuffer(const BufferDescriptor desc, const void* data) {
	Buffer* buffer = new Buffer(desc);
	D3D11_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = data;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;
	device->CreateBuffer(&buffer->apiDesc, &bufferData, &buffer->buffer);
	return buffer;
}

void RenderContext::destroyBuffer(Buffer*& buffer) {
	buffer->buffer->Release();
	delete buffer;
	buffer = nullptr;
}
