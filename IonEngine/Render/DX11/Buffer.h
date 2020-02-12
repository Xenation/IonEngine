#pragma once
#include <d3d11.h>
#include "Render/Descriptors.h"

namespace IonEngine {
	class RenderContext;

	class Buffer {
	public:
		const BufferDescriptor descriptor;

		Buffer(const Buffer&) = delete;
		void operator=(const Buffer&) = delete;

	private:
		ID3D11Buffer* buffer;
		D3D11_BUFFER_DESC apiDesc;

		friend class RenderContext;

		Buffer(const BufferDescriptor desc);
	};
}
