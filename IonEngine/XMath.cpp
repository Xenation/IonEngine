#include "XMath.h"

#include <string>



/* ==== CONSTANTS ==== */
// Vec2i
const Vec2i Vec2i::zero = {0, 0};
const Vec2i Vec2i::one = {1, 1};
const Vec2i Vec2i::right = {1, 0};
const Vec2i Vec2i::left = {-1, 0};
const Vec2i Vec2i::up = {0, 1};
const Vec2i Vec2i::down = {0, -1};
// Vec3i
const Vec3i Vec3i::zero = {0, 0, 0};
const Vec3i Vec3i::one = {1, 1, 1};
const Vec3i Vec3i::right = {1, 0, 0};
const Vec3i Vec3i::left = {-1, 0, 0};
const Vec3i Vec3i::up = {0, 1, 0};
const Vec3i Vec3i::down = {0, -1, 0};
const Vec3i Vec3i::forward = {0, 0, 1};
const Vec3i Vec3i::backward = {0, 0, -1};
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
// Vec2f
const Vec2f Vec2f::zero = {0, 0};
const Vec2f Vec2f::one = {1, 1};
const Vec2f Vec2f::right = {1, 0};
const Vec2f Vec2f::left = {-1, 0};
const Vec2f Vec2f::up = {0, 1};
const Vec2f Vec2f::down = {0, -1};
// Vec3f
const Vec3f Vec3f::zero = {0, 0, 0};
const Vec3f Vec3f::one = {1, 1, 1};
const Vec3f Vec3f::right = {1, 0, 0};
const Vec3f Vec3f::left = {-1, 0, 0};
const Vec3f Vec3f::up = {0, 1, 0};
const Vec3f Vec3f::down = {0, -1, 0};
const Vec3f Vec3f::forward = {0, 0, 1};
const Vec3f Vec3f::backward = {0, 0, -1};
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
// Rotor3f
const Rotor3f Rotor3f::identity = {1, {0, 0, 0}};
// Matrix4x4f
const Matrix4x4f Matrix4x4f::identity = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
// Quaternion
const Quaternion Quaternion::identity = {0.0f, 0.0f, 0.0f, 1.0f};
// Color
const Color Color::clear		= Color(0, 0, 0, 0);
const Color Color::white		= Color(1, 1, 1, 1);
const Color Color::black		= Color(0, 0, 0, 1);
const Color Color::lightGray	= Color(.666f, .666f, .666f, 1);
const Color Color::darkGray		= Color(.333f, .333f, .333f, 1);
const Color Color::blue			= Color(0, 0, 1, 1);
const Color Color::green		= Color(0, 1, 0, 1);
const Color Color::cyan			= Color(0, 1, 1, 1);
const Color Color::red			= Color(1, 0, 0, 1);
const Color Color::magenta		= Color(1, 0, 1, 1);
const Color Color::yellow		= Color(1, 1, 0, 1);
const Color Color::orange		= Color(1, .5f, 0, 1);
const Color Color::lime			= Color(.5f, 1, 0, 1);
const Color Color::turquoise	= Color(0, 1, .5f, 1);
const Color Color::sky			= Color(0, .5f, 1, 1);
const Color Color::purple		= Color(.5f, 0, 1, 1);
const Color Color::pink			= Color(1, 0, .5f, 1);


/* ==== METHODS ==== */
/* ---- Vectors ---- */
// Vec2i
const char* Vec2i::toCString() {
	return toString().c_str();
}
std::string Vec2i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

// Vec3i
const char* Vec3i::toCString() {
	return toString().c_str();
}
std::string Vec3i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

// Vec4i
const char* Vec4i::toCString() {
	return toString().c_str();
}
std::string Vec4i::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

// Vec2f
const char* Vec2f::toCString() {
	return toString().c_str();
}
std::string Vec2f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

// Vec3f
const char* Vec3f::toCString() {
	return toString().c_str();
}
std::string Vec3f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

// Vec4f
const char* Vec4f::toCString() {
	return toString().c_str();
}
std::string Vec4f::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

// Bivec3f
const char* Bivec3f::toCString() {
	return toString().c_str();
}
std::string Bivec3f::toString() {
	return "(xy = " + std::to_string(xy) + ", xz = " + std::to_string(xz) + ", yz = " + std::to_string(yz) + ")";
}

// Rotor3f
const char* Rotor3f::toCString() {
	return toString().c_str();
}
std::string Rotor3f::toString() {
	return "Rotor3f(a = " + std::to_string(a) + ", bv = " + std::string(bv.toString()) + ")";
}

// Quaternion
const char* Quaternion::toCString() {
	return toString().c_str();
}
std::string Quaternion::toString() {
	return "Quaternion(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}


/* ---- Matrices ----*/
// Matrix4x4f
void Matrix4x4f::luDecomposition(Matrix4x4f &lu) {

	float sum = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, j);
			}
			lu.at(i, j) = at(i, j) - sum;
		}
		for (int j = i + 1; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, i);
			}
			lu.at(j, i) = (1.0f / lu.at(i, i)) * (at(j, i) - sum);
		}
	}

}

Vec4f Matrix4x4f::solve(Matrix4x4f &lu, Vec4f toSolve) {

	float sum = 0;
	// find solution of Ly = b
	Vec4f y = {0, 0, 0, 0};
	for (int i = 0; i < 4; i++) {
		sum = 0;
		for (int k = 0; k < i; k++) {
			sum += lu.at(i, k) * y[k];
		}
		y[i] = toSolve[i] - sum;
	}
	// find solution of Ux = y
	Vec4f x = {0, 0, 0, 0};
	for (int i = 3; i >= 0; i--) {
		sum = 0;
		for (int k = i + 1; k < 4; k++) {
			sum += lu.at(i, k) * x[k];
		}
		x[i] = (1.0f / lu.at(i, i)) * (y[i] - sum);
	}

	return x;
}

const char* Matrix4x4f::toCString() {
	return (
		std::to_string(m00) + ", " + std::to_string(m01) + ", " + std::to_string(m02) + ", " + std::to_string(m03) + ",\n" +
		std::to_string(m10) + ", " + std::to_string(m11) + ", " + std::to_string(m12) + ", " + std::to_string(m13) + ",\n" +
		std::to_string(m20) + ", " + std::to_string(m21) + ", " + std::to_string(m22) + ", " + std::to_string(m23) + ",\n" +
		std::to_string(m30) + ", " + std::to_string(m31) + ", " + std::to_string(m32) + ", " + std::to_string(m33) + "\n"
	).c_str();
}
std::string Matrix4x4f::toString() {
	return (
		std::to_string(m00) + ", " + std::to_string(m01) + ", " + std::to_string(m02) + ", " + std::to_string(m03) + ",\n" +
		std::to_string(m10) + ", " + std::to_string(m11) + ", " + std::to_string(m12) + ", " + std::to_string(m13) + ",\n" +
		std::to_string(m20) + ", " + std::to_string(m21) + ", " + std::to_string(m22) + ", " + std::to_string(m23) + ",\n" +
		std::to_string(m30) + ", " + std::to_string(m31) + ", " + std::to_string(m32) + ", " + std::to_string(m33) + "\n"
	);
}

/* ---- Others ---- */
// Color
const char* Color::toCString() {
	return toString().c_str();
}
std::string Color::toString() {
	return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
}

// Boxi
const char* Boxi::toCString() {
	return toString().c_str();
}
std::string Boxi::toString() {
	return "Boxi(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

// Boxf
const char* Boxf::toCString() {
	return toString().c_str();
}
std::string Boxf::toString() {
	return "Boxf(center" + std::string(center.toString()) + ", extents" + std::string(extents.toString()) + ")";
}

// Recti
const char* Recti::toCString() {
	return toString().c_str();
}
std::string Recti::toString() {
	return "Recti(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

// Rectf
const char* Rectf::toCString() {
	return toString().c_str();
}
std::string Rectf::toString() {
	return "Rectf(min" + std::string(min.toString()) + ", max" + std::string(max.toString()) + ")";
}

// Ray
const char* Ray::toCString() {
	return toString().c_str();
}
std::string Ray::toString() {
	return "Ray(origin" + std::string(origin.toString()) + ", direction" + std::string(direction.toString()) + ")";
}

// Plane
const char* Plane::toCString() {
	return toString().c_str();
}
std::string Plane::toString() {
	return "Plane(normal" + std::string(normal.toString()) + ", distance = " + std::to_string(distance) + ")";
}
