#pragma once
#include "Render/Descriptors.h"

namespace IonEngine {
	class Buffer {
	public:
		const BufferDescriptor descriptor;

		Buffer(BufferDescriptor desc);
		Buffer(const Buffer&) = delete;
		void operator=(const Buffer&) = delete;

		u32 getStride();
		u32 getSize();
		u32 getNumElements();

		//void setAPIResource(VkBuffer);
		//VkBuffer getAPIResource();

	private:

	};
}
