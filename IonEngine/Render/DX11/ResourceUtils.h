#pragma once
#include "Render/Descriptors.h"
#include <d3d11.h>

namespace IonEngine {
	static constexpr u32 convertBindFlags(BindFlags flags) {
		switch (flags) {
		case BindFlags::VERTEX_BUFFER: return D3D11_BIND_VERTEX_BUFFER;
		case BindFlags::INDEX_BUFFER: return D3D11_BIND_INDEX_BUFFER;
		case BindFlags::NONE:
		default:
			break;
		}
	}

	static constexpr D3D11_USAGE convertResourceUsage(ResourceUsage usage) {
		switch (usage) {
		case ResourceUsage::DEFAULT: return D3D11_USAGE_DEFAULT;
		case ResourceUsage::IMMUTABLE: return D3D11_USAGE_IMMUTABLE;
		case ResourceUsage::DYNAMIC: return D3D11_USAGE_DYNAMIC;
		case ResourceUsage::STAGING: return D3D11_USAGE_STAGING;
		}
	}
}
