#pragma once
#include "XMath.h"
#include "ZOrder.h"

namespace IonEngine {
	struct Vec2iHash {
		size_t operator()(const Vec2i& v) const {
			size_t zo;
			zo = static_cast<size_t>(zorder((v.x >> 24) & 0xffL, (v.y >> 24) & 0xffL)) << 48;
			zo |= static_cast<size_t>(zorder((v.x >> 16) & 0xffL, (v.y >> 16) & 0xffL)) << 32;
			zo |= static_cast<size_t>(zorder((v.x >> 8) & 0xffL, (v.y >> 8) & 0xffL)) << 16;
			zo |= static_cast<size_t>(zorder(v.x & 0xff, v.y & 0xff));
			return zo;
		}
	};

	struct Vec3iHash {
		size_t operator()(const Vec3i& v) const {
			size_t zo;
			zo = static_cast<size_t>(zorder((v.x >> 16) & 0xffL, (v.y >> 16) & 0xffL, (v.z >> 16) & 0xffL)) << 48;
			zo |= static_cast<size_t>(zorder((v.x >> 8) & 0xffL, (v.y >> 8) & 0xffL, (v.z >> 8) & 0xffL)) << 24;
			zo |= static_cast<size_t>(zorder(v.x & 0xffL, v.y & 0xffL, v.z & 0xffL));
			return zo;
		}
	};

	struct Vec2fHash {
		size_t operator()(const Vec2f& v) const {
			return (*reinterpret_cast<size_t*>(const_cast<float*>(&v.x)) << 32) | (*reinterpret_cast<size_t*>(const_cast<float*>(&v.y)));
		}
	};

	struct Vec3fHash {
		const u32 hashComponentMask = 0x001fffff;
		size_t operator()(const Vec3f& v) const {
			return ((*reinterpret_cast<size_t*>(const_cast<float*>(&v.x)) & hashComponentMask) << 42) | ((*reinterpret_cast<size_t*>(const_cast<float*>(&v.y)) & hashComponentMask) << 21) | (*reinterpret_cast<size_t*>(const_cast<float*>(&v.z)) & hashComponentMask);
		}
	};

	struct Vec4fHash {
		const u32 hashComponentMask = 0x0000ffff;
		size_t operator()(const Vec4f& v) const {
			return ((*reinterpret_cast<size_t*>(const_cast<float*>(&v.x)) & hashComponentMask) << 48) | ((*reinterpret_cast<size_t*>(const_cast<float*>(&v.y)) & hashComponentMask) << 32) | ((*reinterpret_cast<size_t*>(const_cast<float*>(&v.z)) & hashComponentMask) << 16) | (*reinterpret_cast<size_t*>(const_cast<float*>(&v.w)) & hashComponentMask);
		}
	};
}
