#pragma once
#include <Core/MathVec.h>

namespace IonEngine {

	ION_ENUM(Dimension, u32,
		e1D,
		e2D,
		e3D,
	);

	ION_ENUM(ImageFormat, u32,
		UNKNOWN,
		A8B8G8R8_SINT_PACK32,
		A8B8G8R8_SNORM_PACK32,
		A8B8G8R8_SRGB_PACK32,
		A8B8G8R8_UINT_PACK32,
		A8B8G8R8_UNORM_PACK32,
		B10G11R11_UFLOAT_PACK32,
		B8G8R8A8_SRGB,
		B8G8R8A8_UNORM,
		D16_UNORM,
		D32_SFLOAT,
		D32_SFLOAT_S8_UINT,
		R8_SINT,
		R8_SNORM,
		R8_UINT,
		R8_UNORM,
		R8G8_SINT,
		R8G8_SNORM,
		R8G8_UINT,
		R8G8_UNORM,
		R8G8B8A8_SINT,
		R8G8B8A8_SNORM,
		R8G8B8A8_SRGB,
		R8G8B8A8_UINT,
		R8G8B8A8_UNORM,
		R16_SFLOAT,
		R16_SINT,
		R16_UINT,
		R16G16_SFLOAT,
		R16G16_SINT,
		R16G16_UINT,
		R16G16B16A16_SFLOAT,
		R16G16B16A16_SINT,
		R16G16B16A16_UINT,
		R32_SFLOAT,
		R32_SINT,
		R32_UINT,
		R32G32_SFLOAT,
		R32G32_SINT,
		R32G32_UINT,
		R32G32B32A32_SFLOAT,
		R32G32B32A32_SINT,
		R32G32B32A32_UINT,
	);

	ION_ENUM(ResourceUsage, u32,
		DEFAULT,	// GPU read/write
		IMMUTABLE,	// CPU Init only, GPU read only
		DYNAMIC,	// CPU write only, GPU read only
		STAGING,	// GPU to CPU copy
	);

	ION_ENUM(BindFlags, u32,
		NONE = 0,
		VERTEX_BUFFER = 0b0000'0001,
		INDEX_BUFFER = 0b0000'0010,
	);


	struct BufferDescriptor {
		ResourceUsage usage;
		u32 size;
		u32 stride;
		BindFlags bindFlags;
	};

	struct ImageDescriptor {
		Dimension dimension;
		u32 width;
		u32 height;
		u32 depth;
		ImageFormat format;
		u32 arraySize;
	};
}
