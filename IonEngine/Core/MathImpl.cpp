#include "MathUtils.h"
#include "MathVec.h"
#include "MathAdv.h"

using namespace IonEngine;

/* ==== CONSTANTS ==== */
// Vec2i
const Vec2i Vec2i::zero = {0, 0};
const Vec2i Vec2i::one = {1, 1};
const Vec2i Vec2i::right = {1, 0};
const Vec2i Vec2i::left = {-1, 0};
const Vec2i Vec2i::up = {0, 1};
const Vec2i Vec2i::down = {0, -1};
const Vec2i Vec2i::max = {INT_MAX, INT_MAX};
const Vec2i Vec2i::min = {INT_MIN, INT_MIN};
// Vec3i
const Vec3i Vec3i::zero = {0, 0, 0};
const Vec3i Vec3i::one = {1, 1, 1};
const Vec3i Vec3i::right = {1, 0, 0};
const Vec3i Vec3i::left = {-1, 0, 0};
const Vec3i Vec3i::up = {0, 1, 0};
const Vec3i Vec3i::down = {0, -1, 0};
const Vec3i Vec3i::forward = {0, 0, 1};
const Vec3i Vec3i::backward = {0, 0, -1};
const Vec3i Vec3i::max = {INT_MAX, INT_MAX, INT_MAX};
const Vec3i Vec3i::min = {INT_MIN, INT_MIN, INT_MIN};
const Vec3i Vec3i::north = {0, 0, 1};
const Vec3i Vec3i::east = {1, 0, 0};
const Vec3i Vec3i::south = {0, 0, -1};
const Vec3i Vec3i::west = {-1, 0, 0};
// Vec4i
const Vec4i Vec4i::zero = {0, 0, 0, 0};
const Vec4i Vec4i::one = {1, 1, 1, 1};
const Vec4i Vec4i::one3 = {1, 1, 1, 0};
const Vec4i Vec4i::right = {1, 0, 0, 0};
const Vec4i Vec4i::left = {-1, 0, 0, 0};
const Vec4i Vec4i::up = {0, 1, 0, 0};
const Vec4i Vec4i::down = {0, -1, 0, 0};
const Vec4i Vec4i::forward = {0, 0, 1, 0};
const Vec4i Vec4i::backward = {0, 0, -1, 0};
const Vec4i Vec4i::max = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};
const Vec4i Vec4i::min = {INT_MIN, INT_MIN, INT_MIN, INT_MIN};
// Vec2f
const Vec2f Vec2f::zero = {0, 0};
const Vec2f Vec2f::one = {1, 1};
const Vec2f Vec2f::right = {1, 0};
const Vec2f Vec2f::left = {-1, 0};
const Vec2f Vec2f::up = {0, 1};
const Vec2f Vec2f::down = {0, -1};
const Vec2f Vec2f::positiveInfinity = {IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity};
const Vec2f Vec2f::negativeInfinity = {IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity};
const Vec2f Vec2f::nan = {IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan};
// Vec3f
const Vec3f Vec3f::zero = {0, 0, 0};
const Vec3f Vec3f::one = {1, 1, 1};
const Vec3f Vec3f::right = {1, 0, 0};
const Vec3f Vec3f::left = {-1, 0, 0};
const Vec3f Vec3f::up = {0, 1, 0};
const Vec3f Vec3f::down = {0, -1, 0};
const Vec3f Vec3f::forward = {0, 0, 1};
const Vec3f Vec3f::backward = {0, 0, -1};
const Vec3f Vec3f::positiveInfinity = {IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity};
const Vec3f Vec3f::negativeInfinity = {IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity};
const Vec3f Vec3f::nan = {IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan};
// Vec4f
const Vec4f Vec4f::zero = {0, 0, 0, 0};
const Vec4f Vec4f::zeroPoint = {0, 0, 0, 1};
const Vec4f Vec4f::one = {1, 1, 1, 1};
const Vec4f Vec4f::oneDir = {1, 1, 1, 0};
const Vec4f Vec4f::right = {1, 0, 0, 0};
const Vec4f Vec4f::left = {-1, 0, 0, 0};
const Vec4f Vec4f::up = {0, 1, 0, 0};
const Vec4f Vec4f::down = {0, -1, 0, 0};
const Vec4f Vec4f::forward = {0, 0, 1, 0};
const Vec4f Vec4f::backward = {0, 0, -1, 0};
const Vec4f Vec4f::positiveInfinity = {IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity};
const Vec4f Vec4f::negativeInfinity = {IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity};
const Vec4f Vec4f::positiveInfinityPoint = {IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, 1};
const Vec4f Vec4f::negativeInfinityPoint = {IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, 1};
const Vec4f Vec4f::positiveInfinityDir = {IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, IonEngine::Math::f32c::positiveInfinity, 0};
const Vec4f Vec4f::negativeInfinityDir = {IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, IonEngine::Math::f32c::negativeInfinity, 0};
const Vec4f Vec4f::nan = {IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan, IonEngine::Math::f32c::nan};
// Rotor3f
const Rotor3f Rotor3f::identity = {1, {0, 0, 0}};
// Matrix4x4f
const Matrix4x4f Matrix4x4f::identity = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
// Color
const Color Color::clear = Color(0, 0, 0, 0);
const Color Color::white = Color(1, 1, 1, 1);
const Color Color::black = Color(0, 0, 0, 1);
const Color Color::lightGray = Color(.666f, .666f, .666f, 1);
const Color Color::darkGray = Color(.333f, .333f, .333f, 1);
const Color Color::blue = Color(0, 0, 1, 1);
const Color Color::green = Color(0, 1, 0, 1);
const Color Color::cyan = Color(0, 1, 1, 1);
const Color Color::red = Color(1, 0, 0, 1);
const Color Color::magenta = Color(1, 0, 1, 1);
const Color Color::yellow = Color(1, 1, 0, 1);
const Color Color::orange = Color(1, .5f, 0, 1);
const Color Color::lime = Color(.5f, 1, 0, 1);
const Color Color::turquoise = Color(0, 1, .5f, 1);
const Color Color::sky = Color(0, .5f, 1, 1);
const Color Color::purple = Color(.5f, 0, 1, 1);
const Color Color::pink = Color(1, 0, .5f, 1);

