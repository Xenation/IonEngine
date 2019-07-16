#pragma once
#include "XMath.h"
#include "ZOrder.h"

struct Vec2iHash {
	size_t operator()(const Vec2i& v) const {
		unsigned long long zo;
		zo = zorder((v.x >> 24) & 0xff, (v.y >> 24) & 0xff) << 48;
		zo |= zorder((v.x >> 16) & 0xff, (v.y >> 16) & 0xff) << 32;
		zo |= zorder((v.x >> 8) & 0xff, (v.y >> 8) & 0xff) << 16;
		zo |= zorder(v.x & 0xff, v.y & 0xff);
		return (size_t) zo;
	}
};

struct Vec3iHash {
	size_t operator()(const Vec3i& v) const {
		unsigned long long zo;
		zo = zorder((v.x >> 16) & 0xff, (v.y >> 16) & 0xff, (v.z >> 16) & 0xff) << 48;
		zo |= zorder((v.x >> 8) & 0xff, (v.y >> 8) & 0xff, (v.z >> 8) & 0xff) << 24;
		zo |= zorder(v.x & 0xff, v.y & 0xff, v.z & 0xff);
		return (size_t) zo;
	}
};

struct Vec2fHash {
	size_t operator()(const Vec2f& v) const {
		return (size_t) (*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.x)) << 32) | (*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.y)));
	}
};

struct Vec3fHash {
	const unsigned int hashComponentMask = 0x001fffff;
	size_t operator()(const Vec3f& v) const {
		return (size_t) ((*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.x)) & hashComponentMask) << 42) | ((*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.y)) & hashComponentMask) << 21) | (*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.z)) & hashComponentMask);
	}
};

struct Vec4fHash {
	const unsigned int hashComponentMask = 0x0000ffff;
	size_t operator()(const Vec4f& v) const {
		return (size_t) ((*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.x)) & hashComponentMask) << 48) | ((*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.y)) & hashComponentMask) << 32) | ((*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.z)) & hashComponentMask) << 16) | (*reinterpret_cast<unsigned int*>(const_cast<float*>(&v.w)) & hashComponentMask);
	}
};