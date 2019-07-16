#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
//#include <xmmintrin.h> // SSE
//#include <emmintrin.h> // SSE2
//#include <pmmintrin.h> // SSE3
//#include <tmmintrin.h> // SSSE3
#include <smmintrin.h> // SSE4.1
#include <string> // TODO try to separate from this file

#pragma region Constants
const __m128 _mm_one = _mm_set1_ps(1.0f);
const __m128 _mm_negOne = _mm_set1_ps(-1.0f);
const __m128 _mm_mask_signf = _mm_castsi128_ps(_mm_set1_epi32(0x80'00'00'00));
#pragma endregion

#pragma region MathFunctions
inline float sqrtfInline(float x) {
	if (x <= 0) {
		return 0;
	}
	int exp = 0;
	x = frexpf(x, &exp);
	if (exp & 1) {
		exp--;
		x *= 2;
	}
	float y = (1 + x) / 2;
	float z = 0;
	while (y != z) {
		z = y;
		y = (y + x / y) / 2;
	}
	return ldexpf(y, exp / 2);
}

inline float maxf(float a, float b) {
	return (a > b) ? a : b;
}

inline float minf(float a, float b) {
	return (a < b) ? a : b;
}
inline float minf(float a, float b, float c) {
	(a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

inline void minmax3f(float a, float b, float c, float &min, float&max) {
	if (a < b) {
		if (b < c) {
			min = a;
			max = c;
		} else {
			if (a < c) {
				min = a;
				max = b;
			} else {
				min = c;
				max = b;
			}
		}
	} else {
		if (b < c) {
			if (a < c) {
				min = b;
				max = c;
			} else {
				min = b;
				max = a;
			}
		} else {
			min = c;
			max = a;
		}
	}
}

inline double lerpd(double a, double b, double t) {
	return (1 - t) * a + t * b;
}

inline float lerpf(float a, float b, float t) {
	return (1 - t) * a + t * b;
}

inline double faded(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float fadef(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float invsqrt(float n) {
	union {
		float f;
		unsigned int i;
	} conv;

	float x2;
	const float threehalfs = 1.5f;

	x2 = n * 0.5f;
	conv.f = n;
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f = conv.f * (threehalfs - (conv.f * x2 * x2));
	return conv.f;
}

inline int floorToInt(double d) {
	return (d > 0) ? (int) d : (int) (d - 1);
}

inline int floorToInt(float f) {
	return (f > 0) ? (int) f : (int) (f - 1);
}

inline int ceilToInt(double d) {
	return (d > 0) ? ((int) d) + 1 : ((int) (d - 1)) - 1;
}

inline int ceilToInt(float f) {
	return (f > 0) ? ((int) f) + 1 : ((int) (f - 1)) - 1;
}

inline float randomValuef() {
	return rand() / (float) RAND_MAX;
}

inline float randomRangef(float min, float max) {
	// (r / rmax) * (max - min)
	// max * (r / rmax) - min * (r / rmax)
	// r * ((max - min) / rmax)
	return rand() * ((max - min) / (float) RAND_MAX) + min;
}
#pragma endregion

#pragma region Vectors
/* ==== VEC2i ==== */
struct Vec2i {
	union {
		struct {
			int x, y;
		};
		int data[2];
	};
public:
	/* ---- CONSTANTS ---- */
	static const Vec2i zero;
	static const Vec2i one;
	static const Vec2i right;
	static const Vec2i left;
	static const Vec2i up;
	static const Vec2i down;

	/* ---- CONSTRUCTORS ---- */
	inline Vec2i() : x(0), y(0) {}
	inline Vec2i(int x, int y) : x(x), y(y) {}

	/* ---- OPERATORS ---- */
	// Misc
	inline Vec2i operator-() const {
		return Vec2i(-x, -y);
	}

	// Add/Sub
	inline Vec2i operator+(const Vec2i &other) const {
		return {this->x + other.x, this->y + other.y};
	}
	inline Vec2i operator-(const Vec2i &other) const {
		return {this->x - other.x, this->y - other.y};
	}
	inline void operator+=(const Vec2i &other) {
		this->x += other.x;
		this->y += other.y;
	}
	inline void operator-=(const Vec2i &other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Mul/Div
	inline friend Vec2i operator*(const Vec2i &a, const Vec2i &b) {
		return {a.x * b.x , a.y * b.y};
	}
	inline friend Vec2i operator*(const Vec2i &vec, int mult) {
		return {vec.x * mult, vec.y * mult};
	}
	inline friend Vec2i operator*(int mult, const Vec2i &vec) {
		return {vec.x * mult, vec.y * mult};
	}

	inline friend Vec2i operator/(const Vec2i &a, const Vec2i &b) {
		return {a.x / b.x , a.y / b.y};
	}
	inline friend Vec2i operator/(const Vec2i &vec, int mult) {
		return {vec.x / mult, vec.y / mult};
	}
	inline friend Vec2i operator/(int mult, const Vec2i &vec) {
		return {mult / vec.x, mult / vec.y};
	}

	inline void operator*=(const Vec2i& o) {
		x *= o.x;
		y *= o.y;
	}
	inline void operator*=(int o) {
		x *= o;
		y *= o;
	}
	inline void operator/=(const Vec2i& o) {
		x /= o.x;
		y /= o.y;
	}
	inline void operator/=(int o) {
		x /= o;
		y /= o;
	}

	// Comp
	inline bool operator==(const Vec2i &other) const {
		return (this->x == other.x) && (this->y == other.y);
	}
	inline bool operator!=(const Vec2i &other) const {
		return (this->x != other.x) || (this->y != other.y);
	}

	// Index
	inline const int& operator[](int index) const {
		return data[index];
	}
	inline int& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
};

/* ==== VEC3i ==== */
struct Vec3i {
	union {
		struct {
			int x, y, z;
		};
		struct {
			Vec2i xy;
			float _z;
		};
		int data[3];
	};
public:
	/* ---- CONSTANTS ---- */
	static const Vec3i zero;
	static const Vec3i one;
	static const Vec3i right;
	static const Vec3i left;
	static const Vec3i up;
	static const Vec3i down;
	static const Vec3i forward;
	static const Vec3i backward;

	static const Vec3i north;
	static const Vec3i east;
	static const Vec3i south;
	static const Vec3i west;

	/* ---- CONSTRUCTORS ---- */
	inline Vec3i() : x(0), y(0), z(0) {}
	inline Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

	/* ---- OPERATORS ---- */
	// Misc
	inline Vec3i operator-() const {
		return Vec3i(-x, -y, -z);
	}

	// Add/Sub
	inline Vec3i operator+(const Vec3i& o) const {
		return Vec3i(x + o.x, y + o.y, z + o.z);
	}
	inline Vec3i  operator-(const Vec3i& o) const {
		return Vec3i(x - o.x, y - o.y, z - o.z);
	}
	inline void operator+=(const Vec3i& o) {
		x += o.x;
		y += o.y;
		z += o.z;
	}
	inline void operator-=(const Vec3i& o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
	}

	// Mul/Div
	inline friend Vec3i operator*(const Vec3i& a, const Vec3i& b) {
		return Vec3i(a.x * b.x, a.y * b.y, a.z * b.z);
	}
	inline friend Vec3i operator*(const Vec3i& vec, int mult) {
		return Vec3i(vec.x * mult, vec.y * mult, vec.z * mult);
	}
	inline friend Vec3i operator*(int mult, const Vec3i &vec) {
		return Vec3i(mult * vec.x, mult * vec.y, mult * vec.z);
	}

	inline friend Vec3i operator/(const Vec3i& a, const Vec3i& b) {
		return Vec3i(a.x / b.x , a.y / b.y, a.z / b.z);
	}
	inline friend Vec3i operator/(const Vec3i& vec, int mult) {
		return Vec3i(vec.x / mult, vec.y / mult, vec.z / mult);
	}
	inline friend Vec3i operator/(int mult, const Vec3i& vec) {
		return Vec3i(mult / vec.x, mult / vec.y, mult / vec.z);
	}

	inline void operator*=(const Vec3i& o) {
		x *= o.x;
		y *= o.y;
		z *= o.z;
	}
	inline void operator*=(int o) {
		x *= o;
		y *= o;
		z *= o;
	}
	inline void operator/=(const Vec3i& o) {
		x /= o.x;
		y /= o.y;
		z /= o.z;
	}
	inline void operator/=(int o) {
		x /= o;
		y /= o;
		z /= o;
	}

	// Comp
	inline bool operator==(const Vec3i& other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}
	inline bool operator!=(const Vec3i& other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
	}

	// Index
	inline const int& operator[](int index) const {
		return data[index];
	}
	inline int& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
};

/* ==== VEC4i ==== */
struct Vec4i {
	union {
		struct {
			int x, y, z, w;
		};
		struct {
			Vec3i xyz;
			float _w;
		};
		int data[4];
		__m128i _emm;
	};

public:
	/* ---- CONSTANTS ---- */
	static const Vec4i zero;
	static const Vec4i one;
	static const Vec4i one3;
	static const Vec4i right;
	static const Vec4i left;
	static const Vec4i up;
	static const Vec4i down;
	static const Vec4i forward;
	static const Vec4i backward;

	/* ---- CONSTRUCTORS ---- */
	inline Vec4i() : x(0), y(0), z(0), w(0) {}
	inline Vec4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	inline Vec4i(__m128i emm) : _emm(emm) {}

	/* ---- OPERATORS ---- */
	// Misc
	inline Vec4i operator-() const {
		return Vec4i(-x, -y, -z, -w);
	}

	// Add/Sub
	inline friend Vec4i operator+(const Vec4i& a, const Vec4i& b) {
		return Vec4i(_mm_add_epi32(a._emm, b._emm));
	}
	inline friend Vec4i operator-(const Vec4i& a, const Vec4i& b) {
		return Vec4i(_mm_sub_epi32(a._emm, b._emm));
	}
	inline Vec4i operator+=(const Vec4i& o) {
		_emm = _mm_add_epi32(_emm, o._emm);
	}
	inline Vec4i operator-=(const Vec4i& o) {
		_emm = _mm_sub_epi32(_emm, o._emm);
	}

	// Mul/Div
	inline friend Vec4i operator*(const Vec4i& a, const Vec4i& b) {
		return Vec4i(_mm_mul_epi32(a._emm, b._emm));
	}
	inline friend Vec4i operator*(const Vec4i& a, int b) {
		return Vec4i(_mm_mul_epi32(a._emm, _mm_set1_epi32(b)));
	}
	inline friend Vec4i operator*(int a, const Vec4i& b) {
		return Vec4i(_mm_mul_epi32(_mm_set1_epi32(a), b._emm));
	}

	inline friend Vec4i operator/(const Vec4i& a, const Vec4i& b) {
		return Vec4i(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}
	inline friend Vec4i operator/(const Vec4i& a, int b) {
		return Vec4i(a.x / b, a.y / b, a.z / b, a.w / b);
	}
	inline friend Vec4i operator/(int a, const Vec4i& b) {
		return Vec4i(a / b.x, a / b.y, a / b.z, a / b.w);
	}

	inline void operator*=(const Vec4i& o) {
		_emm = _mm_mul_epi32(_emm, o._emm);
	}
	inline void operator*=(int o) {
		_emm = _mm_mul_epi32(_emm, _mm_set1_epi32(o));
	}
	inline void operator/=(const Vec4i& o) {
		x /= o.x;
		y /= o.y;
		z /= o.z;
		w /= o.w;
	}
	inline void operator/=(int o) {
		x /= o;
		y /= o;
		z /= o;
		w /= o;
	}

	// Comp
	inline bool operator==(const Vec4i& o) const {
		return (x == o.x) && (y == o.y) && (z == o.z) && (w == o.w);
	}
	inline bool operator!=(const Vec4i& o) const {
		return (x != o.x) || (y != o.y) || (z != o.z) || (w != o.w);
	}

	// Index
	inline const int& operator[](int index) const {
		return data[index];
	}
	inline int& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
};

/* ==== VEC2f ==== */
struct Vec2f {
	union {
		struct {
			float x, y;
		};
		float data[2];
	};

public:
	/* ---- CONSTANTS ---- */
	static const Vec2f zero;
	static const Vec2f one;
	static const Vec2f right;
	static const Vec2f left;
	static const Vec2f up;
	static const Vec2f down;

	/* ---- CONSTRUCTORS ---- */
	constexpr Vec2f() : x(0.0f), y(0.0f) {}
	constexpr Vec2f(float x, float y) : x(x), y(y) {}
	
	/* ---- OPERATORS ---- */
	// Misc
	inline Vec2f operator-() const {
		return {-this->x, -this->y};
	}

	// Add/Sub
	inline Vec2f operator+(const Vec2f &other) const {
		return {this->x + other.x, this->y + other.y};
	}
	inline Vec2f operator-(const Vec2f &other) const {
		return {this->x - other.x, this->y - other.y};
	}

	inline void operator+=(const Vec2f &other) {
		this->x += other.x;
		this->y += other.y;
	}
	inline void operator-=(const Vec2f &other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Mul/Div
	inline friend Vec2f operator*(const Vec2f &a, const Vec2f &b) {
		return {a.x * b.x , a.y * b.y};
	}
	inline friend Vec2f operator*(const Vec2f &vec, float scalar) {
		return {vec.x * scalar, vec.y * scalar};
	}
	inline friend Vec2f operator*(float scalar, const Vec2f &vec) {
		return {vec.x * scalar, vec.y * scalar};
	}

	inline friend Vec2f operator/(const Vec2f &a, const Vec2f &b) {
		return {a.x / b.x , a.y / b.y};
	}
	inline friend Vec2f operator/(const Vec2f &vec, float scalar) {
		return {vec.x / scalar, vec.y / scalar};
	}
	inline friend Vec2f operator/(float scalar, const Vec2f &vec) {
		return {scalar / vec.x, scalar / vec.y};
	}

	inline void operator*=(const Vec2f& o) {
		x *= o.x;
		y *= o.y;
	}
	inline void operator*=(float o) {
		x *= o;
		y *= o;
	}
	inline void operator/=(const Vec2f& o) {
		x /= o.x;
		y /= o.y;
	}
	inline void operator/=(float o) {
		x /= o;
		y /= o;
	}

	// Comp
	inline bool operator==(const Vec2f &other) const {
		return (this->x == other.x) && (this->y == other.y);
	}
	inline bool operator!=(const Vec2f &other) const {
		return (this->x != other.x) || (this->y != other.y);
	}

	// Index
	inline const float& operator[](int index) const {
		return data[index];
	}
	inline float& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
};

/* ==== VEC3f ==== */
struct Vec3f {
	union {
		struct {
			float x, y, z;
		};
		struct {
			Vec2f xy;
			float _z;
		};
		float data[3];
	};

public:
	/* ---- CONSTANTS ---- */
	static const Vec3f zero;
	static const Vec3f one;
	static const Vec3f right;
	static const Vec3f left;
	static const Vec3f up;
	static const Vec3f down;
	static const Vec3f forward;
	static const Vec3f backward;

	/* ---- CONSTRUCTORS ---- */
	constexpr Vec3f() : x(0.0f), y(0.0f), z(0.0f) { }
	constexpr Vec3f(float x, float y, float z) : x(x), y(y), z(z) { }
	
	/* ---- OPERATORS ---- */
	// Misc
	inline Vec3f operator-() const {
		return Vec3f(-x, -y, -z);
	}

	// Add/Sub
	inline Vec3f operator+(const Vec3f &other) const {
		return Vec3f(x + other.x, y + other.y, z + other.z);
	}
	inline Vec3f operator-(const Vec3f &other) const {
		return Vec3f(x - other.x, y - other.y, z - other.z);
	}

	inline void operator+=(const Vec3f &other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}
	inline void operator-=(const Vec3f &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	// Mul/Div
	inline friend Vec3f operator*(const Vec3f &a, const Vec3f &b) {
		return Vec3f(a.x * b.x, a.y * b.y, a.z * b.z);
	}
	inline friend Vec3f operator*(const Vec3f &vec, float scalar) {
		return Vec3f(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	inline friend Vec3f operator*(float scalar, const Vec3f &vec) {
		return Vec3f(scalar * vec.x, scalar * vec.y, scalar * vec.z);
	}

	inline friend Vec3f operator/(const Vec3f &a, const Vec3f &b) {
		return Vec3f(a.x / b.x, a.y / b.y, a.z / b.z);
	}
	inline friend Vec3f operator/(const Vec3f &vec, float scalar) {
		return Vec3f(vec.x / scalar, vec.y / scalar, vec.z / scalar);
	}
	inline friend Vec3f operator/(float scalar, const Vec3f &vec) {
		return Vec3f(scalar / vec.x, scalar / vec.y, scalar / vec.z);
	}

	inline void operator*=(const Vec3f& o) {
		x *= o.x;
		y *= o.y;
		z *= o.z;
	}
	inline void operator*=(float o) {
		x *= o;
		y *= o;
		z *= o;
	}
	inline void operator/=(const Vec3f& o) {
		x /= o.x;
		y /= o.y;
		z /= o.z;
	}
	inline void operator/=(float o) {
		x /= o;
		y /= o;
		z /= o;
	}

	// Comp
	inline bool operator==(const Vec3f &other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}
	inline bool operator!=(const Vec3f &other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
	}

	// Index
	inline const float& operator[](int index) const {
		return data[index];
	}
	inline float& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
	inline float dot(const Vec3f &other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	inline Vec3f cross(const Vec3f &other) const {
		return {other.y * this->z - other.z * this->y, other.z * this->x - other.x * this->z, other.x * this->y - other.y * this->x};
	}
	inline Vec3f normalized() const {
		return (*this) * invsqrt(this->dot(*this));
	}
	inline Vec3f normalized_precise() const {
		float mag = this->magnitude();
		return {this->x / mag, this->y / mag, this->z / mag};
	}
	inline void normalize() {
		float inv = invsqrt(x * x + y * y + z * z);
		x *= inv;
		y *= inv;
		z *= inv;
	}
	inline void normalize_precise() {
		float mag = this->magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
	}
	inline float magnitude() const {
		return sqrtfInline(x * x + y * y + z * z);
	}
	inline Vec3f abs() const {
		return Vec3f((x < 0) ? -x : x, (y < 0) ? -y : y, (z < 0) ? -z : z);
	}
	inline Vec3f sign() const {
		return Vec3f((x < 0) ? -1 : 1, (y < 0) ? -1 : 1, (z < 0) ? -1 : 1);
	}
	inline Vec3f step(const Vec3f &edge) const {
		return Vec3f((x < edge.x) ? 0 : 1, (z < edge.z) ? 0 : 1, (z < edge.z) ? 0 : 1);
	}
	inline static Vec3f lerp(const Vec3f &a, const Vec3f &b, float t) {
		return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)};
	}
	inline Vec3f inverse() const {
		return {1.0f / x, 1.0f / y, 1.0f / z};
	}
	inline Vec3i floor() const {
		return {floorToInt(x), floorToInt(y), floorToInt(z)};
	}
};

/* ==== VEC4f ==== */
struct Vec4f {
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			Vec3f xyz;
			float _w;
		};
		float data[4];
		__m128 _xmm;
	};

public:
	/* ---- CONSTANTS ---- */
	static const Vec4f zero;
	static const Vec4f zeroPoint;
	static const Vec4f one;
	static const Vec4f oneDir;
	static const Vec4f right;
	static const Vec4f left;
	static const Vec4f up;
	static const Vec4f down;
	static const Vec4f forward;
	static const Vec4f backward;

	/* ---- CONSTRUCTORS ---- */
	constexpr Vec4f() : x(0), y(0), z(0), w(0) { }
	constexpr Vec4f(Vec3f xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	constexpr Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
	constexpr Vec4f(__m128 xmm) : _xmm(xmm) { }

	/* ---- OPERATOR ---- */
	// Misc
	inline Vec4f operator-() const {
		return {-this->x, -this->y, -this->z, -this->w};
	}

	// Add/Sub
	inline Vec4f operator+(const Vec4f &other) const {
		return Vec4f(_mm_add_ps(_xmm, other._xmm));
	}
	inline Vec4f operator-(const Vec4f &other) const {
		return Vec4f(_mm_sub_ps(_xmm, other._xmm));
	}

	inline void operator+=(const Vec4f &other) {
		_xmm = _mm_add_ps(_xmm, other._xmm);
	}
	inline void operator-=(const Vec4f &other) {
		_xmm = _mm_sub_ps(_xmm, other._xmm);
	}

	// Mul/Div
	inline friend Vec4f operator*(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_mul_ps(lhs._xmm, rhs._xmm));
	}
	inline friend Vec4f operator*(const Vec4f &vec, float scalar) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator*(float scalar, const Vec4f &vec) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}

	inline friend Vec4f operator/(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_div_ps(lhs._xmm, rhs._xmm));
	}
	inline friend Vec4f operator/(const Vec4f &vec, float scalar) {
		return Vec4f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator/(float scalar, const Vec4f &vec) {
		return Vec4f(_mm_div_ps(_mm_set1_ps(scalar), vec._xmm));
	}

	inline void operator*=(const Vec4f& o) {
		_xmm = _mm_mul_ps(_xmm, o._xmm);
	}
	inline void operator*=(float o) {
		_xmm = _mm_mul_ps(_xmm, _mm_set1_ps(o));
	}
	inline void operator/=(const Vec4f& o) {
		_xmm = _mm_div_ps(_xmm, o._xmm);
	}
	inline void operator/=(float o) {
		_xmm = _mm_div_ps(_xmm, _mm_set1_ps(o));
	}

	// Comp
	inline bool operator==(const Vec4f &other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
	}
	inline bool operator!=(const Vec4f &other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z) || (this->w != other.w);
	}
	
	// Index
	inline const float& operator[](int index) const {
		return data[index];
	}
	inline float& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
};

/* ==== BIVEC3f ==== */
struct Bivec3f {
	float xy, xz, yz;

public:
	inline Bivec3f() : xy(0), xz(0), yz(0) {}
	inline Bivec3f(float xy, float xz, float yz) : xy(xy), xz(xz), yz(yz) {}

	inline static Bivec3f wedge(const Vec3f& u, const Vec3f& v) {
		return {
			u.x * v.y - u.y * v.x,	// XY
			u.x * v.z - u.z * v.x,	// XZ
			u.y * v.z - u.z * v.y	// YZ
		};
	}

	const char* toCString();
	std::string toString();
};

/* ==== ROTOR3f ==== */
struct Rotor3f {
	float a;
	union {
		Bivec3f bv;
		struct {
			float xy, xz, yz;
		};
	};

public:
	/* ---- CONSTANTS ---- */
	static const Rotor3f identity;

	/* ---- CONSTRUCTORS ---- */
	inline Rotor3f() : a(1), bv() {}
	inline Rotor3f(float a, float xy, float xz, float yz) : a(a), xy(xy), xz(xz), yz(yz) {}
	inline Rotor3f(float a, const Bivec3f& bv) : a(a), bv(bv) {}
	inline Rotor3f(const Vec3f& from, const Vec3f& to) : a(1 + to.dot(from)), bv(Bivec3f::wedge(to, from)) {
		normalize();
	}

	/* ---- OPERATORS ---- */
	inline Rotor3f operator*(const Rotor3f& r) const {
		const Rotor3f& p = *this;
		return {
			p.a * r.a - p.xy * r.xy - p.xz * r.xz - p.yz * r.yz,
			{
				p.xy * r.a + p.a * r.xy + p.yz * r.xz - p.xz * r.yz,
				p.xz * r.a + p.a * r.xz - p.yz * r.xy + p.xy * r.yz,
				p.yz * r.a + p.a * r.yz + p.xz * r.xy - p.xy * r.xz
			}
		};
	}
	inline friend void operator*=(Rotor3f& a, const Rotor3f& b) {
		a = a * b;
	}

	/* ----- METHODS ---- */
	const char* toCString();
	std::string toString();
	inline static Rotor3f AnglePlane(float angleRad, const Bivec3f& bvPlane) {
		float sina = sinf(angleRad / 2.f);
		return {
			cosf(angleRad / 2.f),
			{
				Bivec3f(-sina * bvPlane.xy, -sina * bvPlane.xz, -sina * bvPlane.yz)
			}
		};
	}
	inline Vec3f rotate(const Vec3f& v) const {
		const Rotor3f& p = *this;

		Vec3f q;
		q.x = p.a * v.x + v.y * p.xy + v.z * p.xz;
		q.y = p.a * v.y - v.x * p.xy + v.z * p.yz;
		q.z = p.a * v.z - v.x * p.xz - v.y * p.yz;

		float q012 = -v.x * p.yz + v.y * p.xz - v.z * p.xy;

		return  {
			p.a * q.x + q.y * p.xy + q.z * p.xz - q012 * p.yz,
			p.a * q.y - q.x * p.xy + q012 * p.xz + q.z * p.yz,
			p.a * q.z - q012 * p.xy - q.x * p.xz - q.y * p.yz
		};
	}
	inline Rotor3f rotate(const Rotor3f& r) const {
		return (*this) * r * reverse();
	}
	inline Rotor3f reverse() const {
		return Rotor3f(a, -xy, -xz, -yz);
	}
	inline float magnitudeSqr() const {
		return a * a + xy * xy + xz * xz + yz * yz;
	}
	inline float magnitude() const {
		return sqrtfInline(magnitudeSqr());
	}
	inline void normalize() {
		float isqrt = invsqrt(magnitudeSqr());
		a *= isqrt;
		xy *= isqrt;
		xz *= isqrt;
		yz *= isqrt;
	}
	inline Rotor3f normalized() const {
		float isqrt = invsqrt(magnitudeSqr());
		return {
			a * isqrt,
			{
				xy * isqrt,
				xz * isqrt,
				yz * isqrt
			}
		};
	}
};

/* ==== QUATERNION ==== */
struct Quaternion {
	float x, y, z, w;
public:
	/* ---- CONSTANTS ---- */
	static const Quaternion identity;

	/* ---- CONSTRUCTORS ---- */
	inline Quaternion() : x(0), y(0), z(0), w(1) { }
	inline Quaternion(const float &x, const float &y, const float &z, const float &w) : x(x), y(y), z(z), w(w) {}

	/* ---- OPERATORS ---- */
	// Mul
	inline Quaternion operator*(const Quaternion& b) {
		return {
			b.w * this->x + b.x * this->w - b.y * this->z + b.z * this->y,
			b.w * this->y + b.x * this->z + b.y * this->w - b.z * this->x,
			b.w * this->z - b.x * this->y + b.y * this->x + b.z * this->w,
			b.w * this->w - b.x * this->x - b.y * this->y - b.z * this->z
		};
	}
	inline friend void operator*=(Quaternion& a, const Quaternion& b) {
		a = a * b;
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();
	inline Quaternion normalized() const {
		float invMag = invsqrt(x * x + y * y + z * z + w * w);
		return {x * invMag, y * invMag, z * invMag, w * invMag};
	}
	inline void normalize() {
		float invMag = invsqrt(x * x + y * y + z * z + w * w);
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}
	inline static Quaternion euler(const Vec3f& eulerRot) {
		return headingAttitudeBank(eulerRot.y, eulerRot.z, eulerRot.x);
	}
	inline static Quaternion attitude(const float& pitch, const float& roll, const float& yaw) {
		return headingAttitudeBank(yaw, roll, pitch);
	}
	inline static Quaternion headingAttitudeBank(const float& heading, const float& attitude, const float& bank) {
		float c1 = cosf(heading * 0.5f);
		float s1 = sinf(heading * 0.5f);
		float c2 = cosf(attitude * 0.5f);
		float s2 = sinf(attitude * 0.5f);
		float c3 = cosf(bank * 0.5f);
		float s3 = sinf(bank * 0.5f);
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;

		return {c1c2 * s3 + s1s2 * c3, s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3, c1c2 * c3 - s1s2 * s3};
	}
	inline static Quaternion axisAngle(Vec3f axis, float angle) {
		Quaternion quat;
		float s = sin(angle * 0.5f);
		quat.x = axis.x * s;
		quat.y = axis.y * s;
		quat.z = axis.z * s;
		quat.w = cos(angle * 0.5f);
		return quat;
	}
	inline static Quaternion fromToRotation(Vec3f from, Vec3f to) {
		Quaternion quat;

		float d = from.dot(to);
		if (d >= 1.0f) {
			return Quaternion::identity;
		}
		if (d < 1e-6f - 1.0f) {
			Vec3f axis = Vec3f::right.cross(from);
			if (axis.x == 0.0f && axis.y == 0.0f && axis.z == 0.0f) {
				axis = Vec3f::up.cross(from);
			}
			axis = axis.normalized();
			quat = Quaternion::axisAngle(axis, M_PI);
		} else {
			float invs = invsqrt((1 + d) * 2);
			float s = 1 / invs;
			Vec3f c = from.cross(to);
			quat.x = c.x * invs;
			quat.y = c.y * invs;
			quat.z = c.z * invs;
			quat.w = s * 0.5f;
			quat.normalize();
		}
		return quat;
	}
};

#pragma endregion

#pragma region Matrices
/* ===== MATRIX4x4f ==== */
struct Matrix4x4f {
	union {
		// Stored in column major order but accessed in row major (first digit is row/y, second is column/x)
		struct {
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
		};
		float data[16];
		__m128 _xmm[4];
		Vec4f columns[4];
	};
public:
	/* ---- CONSTANTS ---- */
	static const Matrix4x4f identity;

	/* ---- CONSTRUCTORS ---- */
	constexpr Matrix4x4f() :
		m00(0), m10(0), m20(0), m30(0),
		m01(0), m11(0), m21(0), m31(0),
		m02(0), m12(0), m22(0), m32(0),
		m03(0), m13(0), m23(0), m33(0)
	{ }
	constexpr Matrix4x4f(float m00, float m10, float m20, float m30, float m01, float m11, float m21, float m31, float m02, float m12, float m22, float m32, float m03, float m13, float m23, float m33) :
		m00(m00), m10(m10), m20(m20), m30(m30),
		m01(m01), m11(m11), m21(m21), m31(m31),
		m02(m02), m12(m12), m22(m22), m32(m32),
		m03(m03), m13(m13), m23(m23), m33(m33)
	{ }

	/* ---- OPERATORS ---- */
	inline static void transfColumn(const Matrix4x4f *mat, const __m128 &column, __m128 &res) {
		res = _mm_mul_ps(mat->_xmm[0], _mm_set1_ps(column.m128_f32[0]));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[1], _mm_set1_ps(column.m128_f32[1])));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[2], _mm_set1_ps(column.m128_f32[2])));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[3], _mm_set1_ps(column.m128_f32[3])));
	}
	inline Matrix4x4f operator*(const Matrix4x4f &other) const {
		Matrix4x4f m;
		transfColumn(this, other._xmm[0], m._xmm[0]);
		transfColumn(this, other._xmm[1], m._xmm[1]);
		transfColumn(this, other._xmm[2], m._xmm[2]);
		transfColumn(this, other._xmm[3], m._xmm[3]);
		return m;
	}
	inline Vec4f operator*(const Vec4f &other) const {
		Vec4f v;
		transfColumn(this, other._xmm, v._xmm);
		return v;
	}
	inline Matrix4x4f operator*(float scalar) const {
		Matrix4x4f m;
		__m128 scalar_xmm = _mm_set1_ps(scalar);
		transfColumn(this, scalar_xmm, m._xmm[0]);
		transfColumn(this, scalar_xmm, m._xmm[1]);
		transfColumn(this, scalar_xmm, m._xmm[2]);
		transfColumn(this, scalar_xmm, m._xmm[3]);
		return m;
	}

	/* ---- METHODS ---- */
	void luDecomposition(Matrix4x4f &lu);
	static Vec4f solve(Matrix4x4f &lu, Vec4f toSolve);

	// Access
	inline float get(Vec2i pos) {
		return data[pos.y + pos.x * 4];
	}
	inline float set(Vec2i pos, float val) {
		data[pos.y + pos.x * 4] = val;
	}
	inline float& at(int row, int col) {
		return data[row + col * 4];
	}
	inline Vec3f& translation() {
		return columns[3].xyz;
	}
	inline Vec3f scale() {
		return Vec3f(columns[0].xyz.magnitude(), columns[1].xyz.magnitude(), columns[2].xyz.magnitude());
	}
	inline Quaternion rotation() {
		Vec3f sca = scale();
		Matrix4x4f rotMat = Matrix4x4f(
			m00, m10, m20, 0,
			m01, m11, m21, 0,
			m02, m12, m22, 0,
			0, 0, 0, 1
		);
		rotMat.columns[0].xyz /= sca.x;
		rotMat.columns[1].xyz /= sca.y;
		rotMat.columns[2].xyz /= sca.z;

		Quaternion q;
		float tr = rotMat.m00 + rotMat.m11 + rotMat.m22;
		float s;
		if (tr > 0) {
			s = sqrtf(tr + 1.0f) * 2.0f;
			q.w = 0.25f * s;
			q.x = (rotMat.m21 - rotMat.m12) / s;
			q.y = (rotMat.m02 - rotMat.m20) / s;
			q.z = (rotMat.m10 - rotMat.m01) / s;
		} else if (rotMat.m00 > rotMat.m11 && rotMat.m00 > rotMat.m22) {
			s = sqrtf(1.0f + rotMat.m00 - rotMat.m11 - rotMat.m22) * 2.0f;
			q.w = (rotMat.m21 - rotMat.m12) / s;
			q.x = 0.25f * s;
			q.y = (rotMat.m01 + rotMat.m10) / s;
			q.z = (rotMat.m02 + rotMat.m20) / s;
		} else if (rotMat.m11 > rotMat.m22) {
			s = sqrtf(1.0 + rotMat.m11 - rotMat.m00 - rotMat.m22) * 2.0f;
			q.w = (rotMat.m02 - rotMat.m20) / s;
			q.x = (rotMat.m01 + rotMat.m10) / s;
			q.y = 0.25f * s;
			q.z = (rotMat.m12 + rotMat.m21) / s;
		} else {
			s = sqrtf(1.0 + rotMat.m22 - rotMat.m00 - rotMat.m11) * 2.0f;
			q.w = (rotMat.m10 - rotMat.m01) / s;
			q.x = (rotMat.m02 + rotMat.m20) / s;
			q.y = (rotMat.m12 + rotMat.m21) / s;
			q.z = 0.25f * s;
		}
		return q;
	}

	// Write
	inline void setRotationScale(Quaternion rot, Vec3f scale) {
		m00 = 1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z; m10 = 2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w; m20 = 2.0f * rot.x * rot.z - 2.0f * rot.y * rot.w;
		m01 = 2.0f * rot.x * rot.y - 2.0f * rot.z * rot.w; m11 = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z; m21 = 2.0f * rot.y * rot.z + 2.0f * rot.x * rot.w;
		m02 = 2.0f * rot.x * rot.z + 2.0f * rot.y * rot.w; m12 = 2.0f * rot.y * rot.z - 2.0f * rot.x * rot.w; m22 = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y;

		m00 *= scale.x;
		m01 *= scale.x;
		m02 *= scale.x;
		m10 *= scale.y;
		m11 *= scale.y;
		m12 *= scale.y;
		m20 *= scale.z;
		m21 *= scale.z;
		m22 *= scale.z;
	}

	// Optimized Operations
	inline Matrix4x4f inverseAffine() const {
		float a = m11 * m22 - m12 * m21;
		float b = m12 * m20 - m10 * m22;
		float c = m10 * m21 - m11 * m20;
		float det = m00 * a + m01 * b + m02 * c;
		float detInv = 1.0f / det;
		float inv3[3][3] = {
			{a * detInv, (m02 * m21 - m01 * m22) * detInv, (m01 * m12 - m02 * m11) * detInv},
			{b * detInv, (m00 * m22 - m02 * m20) * detInv, (m02 * m10 - m00 * m12) * detInv},
			{c * detInv, (m01 * m20 - m00 * m21) * detInv, (m00 * m11 - m01 * m10) * detInv}
		};
		return Matrix4x4f(
			inv3[0][0], inv3[1][0], inv3[2][0], 0,
			inv3[0][1], inv3[1][1], inv3[2][1], 0,
			inv3[0][2], inv3[1][2], inv3[2][2], 0,
			-inv3[0][0] * m03 + -inv3[0][1] * m13 + -inv3[0][2] * m23, -inv3[1][0] * m03 + -inv3[1][1] * m13 + -inv3[1][2] * m23, -inv3[2][0] * m03 + -inv3[2][1] * m13 + -inv3[2][2] * m23, 1
		);
	}
	inline Vec3f multPoint(Vec3f point) const {
		__m128 vreg;
		vreg = _mm_mul_ps(_xmm[0], _mm_set1_ps(point.x));
		vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[1], _mm_set1_ps(point.y)));
		vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[2], _mm_set1_ps(point.z)));
		vreg = _mm_add_ps(vreg, _xmm[3]);
		return Vec3f(vreg.m128_f32[0], vreg.m128_f32[1], vreg.m128_f32[2]);
	}
	inline Vec3f multDirection(Vec3f dir) const {
		Vec3f v;
		__m128 vreg;
		vreg = _mm_mul_ps(_xmm[0], _mm_set1_ps(dir.x));
		vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[1], _mm_set1_ps(dir.y)));
		vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[2], _mm_set1_ps(dir.z)));
		return Vec3f(vreg.m128_f32[0], vreg.m128_f32[1], vreg.m128_f32[2]);
	}

	// Transformations
	static Matrix4x4f translation(Vec3f transVec) {
		return Matrix4x4f(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			transVec.x, transVec.y, transVec.z, 1
		);
	}
	static Matrix4x4f scale(Vec3f scaleVec) {
		return Matrix4x4f(
			scaleVec.x, 0, 0, 0,
			0, scaleVec.y, 0, 0,
			0, 0, scaleVec.z, 0,
			0, 0, 0, 1
		);
	}
	static Matrix4x4f translationScale(Vec3f transVec, Vec3f scaleVec) {
		return Matrix4x4f(
			scaleVec.x, 0, 0, 0,
			0, scaleVec.y, 0, 0,
			0, 0, scaleVec.z, 0,
			transVec.x, transVec.y, transVec.z, 1
		);
	}
	static Matrix4x4f rotation(Rotor3f rot) {
		// TODO optimize
		Vec3f vx = rot.rotate(Vec3f(1, 0, 0));
		Vec3f vy = rot.rotate(Vec3f(0, 1, 0));
		Vec3f vz = rot.rotate(Vec3f(0, 0, 1));
		return Matrix4x4f(
			vx.x, vy.x, vz.x,	0,
			vx.y, vy.y, vz.y,	0,
			vx.z, vy.z, vz.z,	0,
			0,		0,		0,	1
		);
	}
	static Matrix4x4f rotation(Quaternion rot) {
		return Matrix4x4f(
			1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z,		2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w,		2.0f * rot.x * rot.z - 2.0f * rot.y * rot.w,		0,
			2.0f * rot.x * rot.y - 2.0f * rot.z * rot.w,			1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z, 2.0f * rot.y * rot.z + 2.0f * rot.x * rot.w,		0,
			2.0f * rot.x * rot.z + 2.0f * rot.y * rot.w,			2.0f * rot.y * rot.z - 2.0f * rot.x * rot.w,		1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y, 0,
			0,														0,													0,													1
		);
	}
	static Matrix4x4f rotation(Vec3f euler) {
		float cx = cosf(euler.x);
		float sx = sinf(euler.x);
		float cy = cosf(euler.y);
		float sy = sinf(euler.y);
		float cz = cosf(euler.z);
		float sz = sinf(euler.z);
		return Matrix4x4f(
			cx * cz - sx * sy * sz,		cz * sx + cx * sy * sz,		-(cy * sz),		0,
			-(cy * sx),					cx * cy,					sy,				0,
			cx * sz + cz * sx * sy,		sx * sz - cx * cz * sy,		cy * cz,		0,
			0,							0,							0,				1
		);
	}
	static Matrix4x4f translationRotation(Vec3f transVec, Quaternion rot) {
		return Matrix4x4f(
			1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z, 2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w, 2.0f * rot.x * rot.z - 2.0f * rot.y * rot.w, 0,
			2.0f * rot.x * rot.y - 2.0f * rot.z * rot.w, 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z, 2.0f * rot.y * rot.z + 2.0f * rot.x * rot.w, 0,
			2.0f * rot.x * rot.z + 2.0f * rot.y * rot.w, 2.0f * rot.y * rot.z - 2.0f * rot.x * rot.w, 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y, 0,
			transVec.x, transVec.y, transVec.z, 1
		);
	}
	static Matrix4x4f translationRotation(Vec3f transVec, Rotor3f rot) {
		// TODO optimize
		Vec3f vx = rot.rotate(Vec3f(1, 0, 0));
		Vec3f vy = rot.rotate(Vec3f(0, 1, 0));
		Vec3f vz = rot.rotate(Vec3f(0, 0, 1));
		return Matrix4x4f(
			vx.x, vy.x, vz.x, 0,
			vx.y, vy.y, vz.y, 0,
			vx.z, vy.z, vz.z, 0,
			transVec.x, transVec.y, transVec.z, 1
		);
	}
	static Matrix4x4f translationRotation(Vec3f transVec, Vec3f euler) {
		float cx = cosf(euler.x);
		float sx = sinf(euler.x);
		float cy = cosf(euler.y);
		float sy = sinf(euler.y);
		float cz = cosf(euler.z);
		float sz = sinf(euler.z);
		return Matrix4x4f(
			cx * cz - sx * sy * sz, cz * sx + cx * sy * sz, -(cy * sz), 0,
			-(cy * sx), cx * cy, sy, 0,
			cx * sz + cz * sx * sy, sx * sz - cx * cz * sy, cy * cz, 0,
			transVec.x, transVec.y, transVec.z, 1
		);
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Rotor3f rotor) {
		return translationRotation(translationVec, rotor) * scale(scaleVec);
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Quaternion rotationQuat) {
		return translationRotation(translationVec, rotationQuat) * scale(scaleVec);
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Vec3f rotationVec) {
		return translationRotation(translationVec, rotationVec) * scale(scaleVec);
	}
	static Matrix4x4f perspectiveProjection(float fov, float aspect, float nearPlane, float farPlane) {
		float zRange = farPlane - nearPlane;

		float xScale = 1.0f / tanf(fov * 0.5f);
		float yScale = xScale * aspect;

		return Matrix4x4f(
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, (farPlane + nearPlane) / zRange, 1,
			0, 0, -(2.0f * farPlane * nearPlane) / zRange, 0
		);
	}

	const char* toCString();
	std::string toString();
};
#pragma endregion

#pragma region OtherStructures
/* ==== COLOR ==== */
struct Color {
	union {
		struct {
			float r, g, b, a;
		};
		float data[4];
		Vec4f vec;
		__m128 _reg;
	};

public:
	/* ---- CONSTANTS ---- */
	static const Color clear;
	static const Color white;
	static const Color black;
	static const Color lightGray;
	static const Color darkGray;
	static const Color blue;
	static const Color green;
	static const Color cyan;
	static const Color red;
	static const Color magenta;
	static const Color yellow;
	static const Color orange;
	static const Color lime;
	static const Color turquoise;
	static const Color sky;
	static const Color purple;
	static const Color pink;

	/* ---- CONSTRUCTORS ---- */
	inline Color() : r(0), g(0), b(0), a(1) {}
	inline Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
	inline Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	inline Color(Vec4f vec) : vec(vec) {}
	inline Color(__m128 reg) : _reg(reg) {}

	inline static Color fromHSV(float h, float s, float v) {
		return Color(hsvIntermediate(h, s, v, 5), hsvIntermediate(h, s, v, 3), hsvIntermediate(h, s, v, 1));
	}
	inline static Color fromHSV(float h, float s, float v, float a) {
		return Color(hsvIntermediate(h, s, v, 5), hsvIntermediate(h, s, v, 3), hsvIntermediate(h, s, v, 1), a);
	}

	/* ---- OPERATORS ---- */
	// Add/Sub
	inline friend Color operator+(const Color& a, const Color& b) {
		return Color(a.vec + b.vec);
	}
	inline friend Color operator-(const Color& a, const Color& b) {
		return Color(a.vec - b.vec);
	}

	inline void operator+=(const Color& o) {
		vec += o.vec;
	}
	inline void operator-=(const Color& o) {
		vec -= o.vec;
	}

	// Mul/Div
	inline friend Color operator*(const Color& a, const Color& b) {
		return Color(a.vec * b.vec);
	}
	inline friend Color operator*(const Color& a, float b) {
		return Color(a.vec * b);
	}
	inline friend Color operator*(float a, const Color& b) {
		return Color(a * b.vec);
	}

	inline friend Color operator/(const Color& a, const Color& b) {
		return Color(a.vec / b.vec);
	}
	inline friend Color operator/(const Color& a, float b) {
		return Color(a.vec / b);
	}
	inline friend Color operator/(float a, const Color& b) {
		return Color(a / b.vec);
	}

	inline void operator*=(const Color& o) {
		vec *= o.vec;
	}
	inline void operator*=(float f) {
		vec *= f;
	}
	inline void operator/=(const Color& o) {
		vec /= o.vec;
	}
	inline void operator/=(float f) {
		vec /= f;
	}

	// Comp
	inline bool operator==(const Color& o) const {
		return vec == o.vec;
	}
	inline bool operator!=(const Color& o) const {
		return vec != o.vec;
	}

	// Index
	inline const float& operator[](int index) const {
		return data[index];
	}
	inline float& operator[](int index) {
		return data[index];
	}

	/* ---- METHODS ---- */
	const char* toCString();
	std::string toString();

private:
	inline static float hsvIntermediate(float h, float s, float v, float n) {
		float k = fmodf(n + h / 60.0f, 6.0f);
		return v - v * s * maxf(minf(k, 4 - k, 1), 0);
	}
};

/* ==== BOXi ==== */
struct Boxi {
	Vec3i min, max;

public:
	inline Boxi() : min(Vec3i(0, 0, 0)), max(Vec3i(0, 0, 0)) {}
	inline Boxi(Vec3i min, Vec3i max) : min(min), max(max) {}

	inline static Boxi fromCenterExtents(Vec3i center, Vec3i extents) {
		return Boxi(center - extents, center + extents);
	}

	const char* toCString();
	std::string toString();
};

/* ==== BOXf ==== */
struct Boxf {
	Vec3f center, extents;

public:
	inline Boxf() : center(Vec3f(0, 0, 0)), extents(Vec3f(0, 0, 0)) {}
	inline Boxf(Vec3f center, Vec3f extents) : center(center), extents(extents) {}

	inline static Boxf fromMinMax(Vec3f min, Vec3f max) {
		return Boxf(Vec3f((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f), Vec3f((max.x - min.x) / 2.0f, (max.y - min.y) / 2.0f, (max.z - min.z) / 2.0f));
	}

	const char* toCString();
	std::string toString();
};

/* ==== RECTi ==== */
struct Recti {
	Vec2i min, max;

public:
	inline Recti() : min(Vec2i(0, 0)), max(Vec2i(0, 0)) {}
	inline Recti(Vec2i min, Vec2i max) : min(min), max(max) {}

	const char* toCString();
	std::string toString();
};

/* ==== RECTf ==== */
struct Rectf {
	Vec2f min, max;

public:
	inline Rectf() : min(Vec2f(0, 0)), max(Vec2f(0, 0)) {}
	inline Rectf(Vec2f min, Vec2f max) : min(min), max(max) {}

	const char* toCString();
	std::string toString();
};

/* ==== RAY ==== */
struct Ray {
	Vec3f origin, direction;

public:
	inline Ray() : origin(Vec3f(0, 0, 0)), direction(Vec3f(0, 1, 0)) {}
	inline Ray(Vec3f origin, Vec3f dir) : origin(origin), direction(dir) {}

	const char* toCString();
	std::string toString();
};

/* ==== PLANE ==== */
struct Plane {
	union {
		Vec4f normalDistance;
		struct {
			Vec3f normal;
			float distance;
		};
	};

public:
	inline Plane() : normalDistance(0.0f, 1.0f, 0.0f, 0.0f) {}
	inline Plane(Vec3f normal, float distance) : normal(normal), distance(distance) {}
	inline Plane(Vec4f normalDistance) : normalDistance(normalDistance) {}

	inline bool intersectRay(const Ray& ray, Vec3f& intersect, float& t) const {
		float denom = normalDistance.x * ray.direction.x + normalDistance.y * ray.direction.y + normalDistance.z * ray.direction.z;
		t = (-normalDistance.x * ray.origin.x - normalDistance.y * ray.origin.y - normalDistance.z * ray.origin.z - normalDistance.w) / denom;
		if (t <= 0) {
			return false;
		}
		intersect = ray.origin + ray.direction * t;
		return true;
	}

	const char* toCString();
	std::string toString();
};
#pragma endregion

#pragma region Conversions
// Vec2i
inline Vec2i toVec2i(const Vec3i& v) {
	return Vec2i(v.x, v.y);
}
inline Vec2i toVec2i(const Vec4i& v) {
	return Vec2i(v.x, v.y);
}
inline Vec2i toVec2i(const Vec2f& v) {
	return Vec2i(floorToInt(v.x), floorToInt(v.y));
}
inline Vec2i toVec2i(const Vec3f& v) {
	return Vec2i(floorToInt(v.x), floorToInt(v.y));
}
inline Vec2i toVec2i(const Vec4f& v) {
	return Vec2i(floorToInt(v.x), floorToInt(v.y));
}

// Vec3i
inline Vec3i toVec3i(const Vec2i& v) {
	return Vec3i(v.x, v.y, 0);
}
inline Vec3i toVec3i(const Vec4i& v) {
	return Vec3i(v.x, v.y, v.z);
}
inline Vec3i toVec3i(const Vec2f& v) {
	return Vec3i(floorToInt(v.x), floorToInt(v.y), 0);
}
inline Vec3i toVec3i(const Vec3f& v) {
	return Vec3i(floorToInt(v.x), floorToInt(v.y), floorToInt(v.z));
}
inline Vec3i toVec3i(const Vec4f& v) {
	return Vec3i(floorToInt(v.x), floorToInt(v.y), floorToInt(v.z));
}

// Vec4i
inline Vec4i toVec4i(const Vec2i& v) {
	return Vec4i(v.x, v.y, 0, 0);
}
inline Vec4i toVec4i(const Vec3i& v) {
	return Vec4i(v.x, v.y, v.z, 0);
}
inline Vec4i toVec4i(const Vec2f& v) {
	return Vec4i(floorToInt(v.x), floorToInt(v.y), 0, 0);
}
inline Vec4i toVec4i(const Vec3f& v) {
	return Vec4i(floorToInt(v.x), floorToInt(v.y), floorToInt(v.z), 0);
}
inline Vec4i toVec4i(const Vec4f& v) {
	return Vec4i(floorToInt(v.x), floorToInt(v.y), floorToInt(v.z), floorToInt(v.w));
}

// Vec2f
inline Vec2f toVec2f(const Vec3f& v) {
	return Vec2f(v.x, v.y);
}
inline Vec2f toVec2f(const Vec4f& v) {
	return Vec2f(v.x, v.y);
}
inline Vec2f toVec2f(const Vec2i& v) {
	return Vec2f((float) v.x, (float) v.y);
}
inline Vec2f toVec2f(const Vec3i& v) {
	return Vec2f((float) v.x, (float) v.y);
}
inline Vec2f toVec2f(const Vec4i& v) {
	return Vec2f((float) v.x, (float) v.y);
}

// Vec3f
inline Vec3f toVec3f(const Vec2f& v) {
	return Vec3f(v.x, v.y, 0);
}
inline Vec3f toVec3f(const Vec4f& v) {
	return Vec3f(v.x, v.y, v.z);
}
inline Vec3f toVec3f(const Vec2i& v) {
	return Vec3f((float) v.x, (float) v.y, 0);
}
inline Vec3f toVec3f(const Vec3i& v) {
	return Vec3f((float) v.x, (float) v.y, (float) v.z);
}
inline Vec3f toVec3f(const Vec4i& v) {
	return Vec3f((float) v.x, (float) v.y, (float) v.z);
}

// Vec4f
inline Vec4f toVec4f(const Vec2f& v) {
	return Vec4f(v.x, v.y, 0, 0);
}
inline Vec4f toVec4f(const Vec3f& v) {
	return Vec4f(v.x, v.y, v.z, 0);
}
inline Vec4f toVec4f(const Vec2i& v) {
	return Vec4f((float) v.x, (float) v.y, 0, 0);
}
inline Vec4f toVec4f(const Vec3i& v) {
	return Vec4f((float) v.x, (float) v.y, (float) v.z, 0);
}
inline Vec4f toVec4f(const Vec4i& v) {
	return Vec4f((float) v.x, (float) v.y, (float) v.z, (float) v.w);
}
#pragma endregion

#pragma region Structures Math Functions
// NDC Raster functions
inline float edge2D(const Vec4f &a, const Vec4f &b, const Vec4f &c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

inline bool triangleBarycentric2D(const Vec4f &v0, const Vec4f &v1, const Vec4f &v2, const Vec4f &p, Vec3f &bary) {
	float area = edge2D(v0, v1, v2);
	bary.x = edge2D(v1, v2, p) / area;
	bary.y = edge2D(v2, v0, p) / area;
	bary.z = edge2D(v0, v1, p) / area;
	return bary.x >= 0 && bary.y >= 0 && bary.z >= 0;
}

inline bool triangleBarycentric(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Vec3f& p, Vec3f& bary) {
	float divider = (v2.z - v1.z) * (v0.x - v1.x) + (v1.x - v2.x) * (v0.z - v1.z);
	if (divider == 0) return false;
	float weightV0 = ((v2.z - v1.z) * (p.x - v1.x) + (v1.x - v2.x) * (p.y - v1.z)) / divider;
	float weightV2 = ((v1.z - v0.z) * (p.x - v1.x) + (v0.x - v1.x) * (p.y - v1.z)) / divider;
	float weightV1 = 1.0f - weightV0 - weightV2;
	bary = Vec3f(weightV0, weightV1, weightV2);
	return weightV0 >= 0 && weightV1 >= 0 && weightV2 >= 0;
}

inline bool intersectTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Ray& ray, Vec3f& intersect, float& t) {
	Vec3f v01 = v1 - v0;
	Vec3f v02 = v2 - v0;
	Vec3f normal = v01.cross(v02).normalized();
	Plane triPlane = Plane(normal, -normal.x * v0.x - normal.y * v0.y - normal.z * v0.z);
	if (!triPlane.intersectRay(ray, intersect, t)) {
		return false;
	}
	Vec3f bary;
	return triangleBarycentric(v0, v1, v2, intersect, bary);
}

inline bool intersectQuad(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Ray& ray, Vec3f& intersect, float& t) {
	Vec3f v01 = v1 - v0;
	Vec3f v12 = v2 - v1;
	Vec3f v23 = v3 - v2;
	Vec3f v30 = v0 - v3;
	Vec3f normal = v01.cross(v2 - v0).normalized();
	if (normal.dot(ray.direction) < 0) return false;
	Plane triPlane = Plane(normal, -normal.x * v0.x - normal.y * v0.y - normal.z * v0.z);
	if (!triPlane.intersectRay(ray, intersect, t)) {
		return false;
	}
	Vec3f v0i = intersect - v0;
	Vec3f v1i = intersect - v1;
	Vec3f v2i = intersect - v2;
	Vec3f v3i = intersect - v3;
	Vec3f c0 = v01.cross(v0i);
	Vec3f c1 = v12.cross(v1i);
	Vec3f c2 = v23.cross(v2i);
	Vec3f c3 = v30.cross(v3i);
	return c0.dot(c1) > 0 && c1.dot(c2) > 0 && c2.dot(c3) > 0;
}

inline void swapToMinMax(Vec3i& min, Vec3i& max) {
	int tmp;
	if (min.x > max.x) {
		tmp = min.x;
		min.x = max.x;
		max.x = tmp;
	}
	if (min.y > max.y) {
		tmp = min.y;
		min.y = max.y;
		max.y = tmp;
	}
	if (min.z > max.z) {
		tmp = min.z;
		min.z = max.z;
		max.z = tmp;
	}
}
#pragma endregion