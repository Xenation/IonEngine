#pragma once
#include "MathUtils.h"

namespace IonEngine {
	inline namespace Math {
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
			static const Vec2i max;
			static const Vec2i min;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec2i() : x(0), y(0) {}
			constexpr Vec2i(int x, int y) : x(x), y(y) {}

			/* ---- OPERATORS ---- */
			// Misc
			inline Vec2i operator-() const {
				return Vec2i(-x, -y);
			}

			// Add/Sub
			inline Vec2i operator+(const Vec2i& other) const {
				return {this->x + other.x, this->y + other.y};
			}
			inline Vec2i operator-(const Vec2i& other) const {
				return {this->x - other.x, this->y - other.y};
			}
			inline void operator+=(const Vec2i& other) {
				this->x += other.x;
				this->y += other.y;
			}
			inline void operator-=(const Vec2i& other) {
				this->x -= other.x;
				this->y -= other.y;
			}

			// Mul/Div
			inline friend Vec2i operator*(const Vec2i& a, const Vec2i& b) {
				return {a.x * b.x, a.y * b.y};
			}
			inline friend Vec2i operator*(const Vec2i& vec, int mult) {
				return {vec.x * mult, vec.y * mult};
			}
			inline friend Vec2i operator*(int mult, const Vec2i& vec) {
				return {vec.x * mult, vec.y * mult};
			}

			inline friend Vec2i operator/(const Vec2i& a, const Vec2i& b) {
				return {a.x / b.x, a.y / b.y};
			}
			inline friend Vec2i operator/(const Vec2i& vec, int mult) {
				return {vec.x / mult, vec.y / mult};
			}
			inline friend Vec2i operator/(int mult, const Vec2i& vec) {
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
			inline bool operator==(const Vec2i& other) const {
				return (this->x == other.x) && (this->y == other.y);
			}
			inline bool operator!=(const Vec2i& other) const {
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
			inline int dot(const Vec2i& other) const {
				return x * other.x + y * other.y;
			}
			inline float magnitude() const {
				return sqrt((float) (x * x + y * y));
			}
			inline Vec2i abs() const {
				return Vec2i((x < 0) ? -x : x, (y < 0) ? -y : y);
			}
			inline Vec2i sign() const {
				return Vec2i((x < 0) ? -1 : 1, (y < 0) ? -1 : 1);
			}
			inline Vec2i step(const Vec2i& edge) const {
				return step(*this, edge);
			}
			inline Vec2i clamped(int min, int max) const {
				return Vec2i(clamp(x, min, max), clamp(y, min, max));
			}
			inline Vec2i clamped(Vec2i min, Vec2i max) const {
				return Vec2i(clamp(x, min.x, max.x), clamp(y, min.y, max.y));
			}

			// Static
			inline static int dot(const Vec2i& l, const Vec2i& r) {
				return l.x * r.x + l.y * r.y;
			}
			inline static Vec2i step(const Vec2i& in, const Vec2i& edge) {
				return Vec2i((in.x < edge.x) ? 0 : 1, (in.y < edge.y) ? 0 : 1);
			}
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
			static const Vec3i max;
			static const Vec3i min;

			static const Vec3i north;
			static const Vec3i east;
			static const Vec3i south;
			static const Vec3i west;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec3i() : x(0), y(0), z(0) {}
			constexpr Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

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
			inline friend Vec3i operator*(int mult, const Vec3i& vec) {
				return Vec3i(mult * vec.x, mult * vec.y, mult * vec.z);
			}

			inline friend Vec3i operator/(const Vec3i& a, const Vec3i& b) {
				return Vec3i(a.x / b.x, a.y / b.y, a.z / b.z);
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
			inline int dot(const Vec3i& other) const {
				return x * other.x + y * other.y + z * other.z;
			}
			inline float magnitude() const {
				return sqrt((float) (x * x + y * y + z * z));
			}
			inline Vec3i abs() const {
				return Vec3i((x < 0) ? -x : x, (y < 0) ? -y : y, (z < 0) ? -z : z);
			}
			inline Vec3i sign() const {
				return Vec3i((x < 0) ? -1 : 1, (y < 0) ? -1 : 1, (z < 0) ? -1 : 1);
			}
			inline Vec3i step(const Vec3i& edge) const {
				return step(*this, edge);
			}
			inline Vec3i clamped(int min, int max) const {
				return Vec3i(clamp(x, min, max), clamp(y, min, max), clamp(z, min, max));
			}
			inline Vec3i clamped(Vec3i min, Vec3i max) const {
				return Vec3i(clamp(x, min.x, max.x), clamp(y, min.y, max.y), clamp(z, min.z, max.z));
			}

			// Static
			inline static int dot(const Vec3i& l, const Vec3i& r) {
				return l.x * r.x + l.y * r.y + l.z * r.z;
			}
			inline static Vec3i step(const Vec3i& in, const Vec3i& edge) {
				Vec3i((in.x < edge.x) ? 0 : 1, (in.y < edge.y) ? 0 : 1, (in.z < edge.z) ? 0 : 1);
			}
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
			static const Vec4i max;
			static const Vec4i min;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec4i() : x(0), y(0), z(0), w(0) {}
			constexpr Vec4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
			constexpr Vec4i(__m128i emm) : _emm(emm) {}

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
			inline int dot(const Vec4i& other) const {
				return x * other.x + y * other.y + z * other.z + w * other.w;
			}
			inline float magnitude() const {
				return sqrt((float) (x * x + y * y + z * z + w * w));
			}
			inline Vec4i abs() const {
				return Vec4i((x < 0) ? -x : x, (y < 0) ? -y : y, (z < 0) ? -z : z, (w < 0) ? -w : w);
			}
			inline Vec4i sign() const {
				return Vec4i((x < 0) ? -1 : 1, (y < 0) ? -1 : 1, (z < 0) ? -1 : 1, (w < 0) ? -1 : 1);
			}
			inline Vec4i step(const Vec4i& edge) {
				return step(*this, edge);
			}
			inline Vec4i clamped(int min, int max) const {
				return Vec4i(clamp(x, min, max), clamp(y, min, max), clamp(z, min, max), clamp(w, min, max));
			}
			inline Vec4i clamped(Vec4i min, Vec4i max) const {
				return Vec4i(clamp(x, min.x, max.x), clamp(y, min.y, max.y), clamp(z, min.z, max.z), clamp(w, min.w, max.w));
			}

			// Static
			inline static int dot(const Vec4i& l, const Vec4i& r) {
				return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
			}
			inline static Vec4i step(const Vec4i& in, const Vec4i& edge) {
				return Vec4i((in.x < edge.x) ? 0 : 1, (in.y < edge.y) ? 0 : 1, (in.z < edge.z) ? 0 : 1, (in.w < edge.w) ? 0 : 1);
			}
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
			static const Vec2f positiveInfinity;
			static const Vec2f negativeInfinity;
			static const Vec2f nan;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec2f() : x(0.0f), y(0.0f) {}
			constexpr Vec2f(float x, float y) : x(x), y(y) {}

			/* ---- OPERATORS ---- */
			// Misc
			inline Vec2f operator-() const {
				return {-this->x, -this->y};
			}

			// Add/Sub
			inline Vec2f operator+(const Vec2f& other) const {
				return {this->x + other.x, this->y + other.y};
			}
			inline Vec2f operator-(const Vec2f& other) const {
				return {this->x - other.x, this->y - other.y};
			}

			inline void operator+=(const Vec2f& other) {
				this->x += other.x;
				this->y += other.y;
			}
			inline void operator-=(const Vec2f& other) {
				this->x -= other.x;
				this->y -= other.y;
			}

			// Mul/Div
			inline friend Vec2f operator*(const Vec2f& a, const Vec2f& b) {
				return {a.x * b.x, a.y * b.y};
			}
			inline friend Vec2f operator*(const Vec2f& vec, float scalar) {
				return {vec.x * scalar, vec.y * scalar};
			}
			inline friend Vec2f operator*(float scalar, const Vec2f& vec) {
				return {vec.x * scalar, vec.y * scalar};
			}

			inline friend Vec2f operator/(const Vec2f& a, const Vec2f& b) {
				return {a.x / b.x, a.y / b.y};
			}
			inline friend Vec2f operator/(const Vec2f& vec, float scalar) {
				return {vec.x / scalar, vec.y / scalar};
			}
			inline friend Vec2f operator/(float scalar, const Vec2f& vec) {
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
			inline bool operator==(const Vec2f& other) const {
				return (this->x == other.x) && (this->y == other.y);
			}
			inline bool operator!=(const Vec2f& other) const {
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
			inline float dot(const Vec2f& other) const {
				return x * other.x + y * other.y;
			}
			inline Vec2f normalized() const {
				return (*this) * invsqrt(this->dot(*this));
			}
			inline Vec2f normalized_precise() const {
				float mag = this->magnitude();
				return {this->x / mag, this->y / mag};
			}
			inline void normalize() {
				float inv = invsqrt(x * x + y * y);
				x *= inv;
				y *= inv;
			}
			inline void normalize_precise() {
				float mag = magnitude();
				x /= mag;
				y /= mag;
			}
			inline float magnitude() const {
				return sqrt(x * x + y * y);
			}
			inline Vec2f abs() const {
				return Vec2f((x < 0.0f) ? -x : x, (y < 0.0f) ? -y : y);
			}
			inline Vec2f sign() const {
				return Vec2f((x < 0.0f) ? -1.0f : 1.0f, (y < 0.0f) ? -1.0f : 1.0f);
			}
			inline void inverse() {
				x = 1.0f / x; y = 1.0f / y;
			}
			inline Vec2f inversed() const {
				return Vec2f(1.0f / x, 1.0f / y);
			}
			inline Vec2i floor() const {
				return Vec2i(floorToInt(x), floorToInt(y));
			}
			inline Vec2i ceil() const {
				return Vec2i(ceilToInt(x), ceilToInt(y));
			}
			inline Vec2i round() const {
				return Vec2i(roundToInt(x), roundToInt(y));
			}
			inline Vec2f step(const Vec2f& edge) const {
				return Vec2f((x < edge.x) ? 0.0f : 1.0f, (y < edge.y) ? 0.0f : 1.0f);
			}
			inline Vec2f clamped(float min, float max) const {
				return Vec2f(clamp(x, min, max), clamp(y, min, max));
			}
			inline Vec2f clamped(Vec2f min, Vec2f max) const {
				return Vec2f(clamp(x, min.x, max.x), clamp(y, min.y, max.y));
			}

			// Static
			inline float dot(const Vec2f& l, const Vec2f& r) {
				return l.x * r.x + l.y * r.y;
			}
			inline Vec2f step(const Vec2f& in, const Vec2f& edge) {
				return Vec2f((in.x < edge.x) ? 0.0f : 1.0f, (in.y < edge.y) ? 0.0f : 1.0f);
			}
			inline Vec2f lerp(const Vec2f& a, const Vec2f& b, float t) {
				return Vec2f(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t));
			}
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
			static const Vec3f positiveInfinity;
			static const Vec3f negativeInfinity;
			static const Vec3f nan;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}
			constexpr Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

			/* ---- OPERATORS ---- */
			// Misc
			inline Vec3f operator-() const {
				return Vec3f(-x, -y, -z);
			}

			// Add/Sub
			inline Vec3f operator+(const Vec3f& other) const {
				return Vec3f(x + other.x, y + other.y, z + other.z);
			}
			inline Vec3f operator-(const Vec3f& other) const {
				return Vec3f(x - other.x, y - other.y, z - other.z);
			}

			inline void operator+=(const Vec3f& other) {
				x += other.x;
				y += other.y;
				z += other.z;
			}
			inline void operator-=(const Vec3f& other) {
				x -= other.x;
				y -= other.y;
				z -= other.z;
			}

			// Mul/Div
			inline friend Vec3f operator*(const Vec3f& a, const Vec3f& b) {
				return Vec3f(a.x * b.x, a.y * b.y, a.z * b.z);
			}
			inline friend Vec3f operator*(const Vec3f& vec, float scalar) {
				return Vec3f(vec.x * scalar, vec.y * scalar, vec.z * scalar);
			}
			inline friend Vec3f operator*(float scalar, const Vec3f& vec) {
				return Vec3f(scalar * vec.x, scalar * vec.y, scalar * vec.z);
			}

			inline friend Vec3f operator/(const Vec3f& a, const Vec3f& b) {
				return Vec3f(a.x / b.x, a.y / b.y, a.z / b.z);
			}
			inline friend Vec3f operator/(const Vec3f& vec, float scalar) {
				return Vec3f(vec.x / scalar, vec.y / scalar, vec.z / scalar);
			}
			inline friend Vec3f operator/(float scalar, const Vec3f& vec) {
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
			inline bool operator==(const Vec3f& other) const {
				return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
			}
			inline bool operator!=(const Vec3f& other) const {
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
			inline float dot(const Vec3f& other) const {
				return x * other.x + y * other.y + z * other.z;
			}
			inline Vec3f cross(const Vec3f& other) const {
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
				return sqrt(x * x + y * y + z * z);
			}
			inline Vec3f abs() const {
				return Vec3f((x < 0.0f) ? -x : x, (y < 0.0f) ? -y : y, (z < 0.0f) ? -z : z);
			}
			inline Vec3f sign() const {
				return Vec3f((x < 0.0f) ? -1.0f : 1.0f, (y < 0.0f) ? -1.0f : 1.0f, (z < 0.0f) ? -1.0f : 1.0f);
			}
			inline void inverse() {
				x = 1.0f / x; y = 1.0f / y; z = 1.0f / z;
			}
			inline Vec3f inversed() const {
				return {1.0f / x, 1.0f / y, 1.0f / z};
			}
			inline Vec3i floor() const {
				return {floorToInt(x), floorToInt(y), floorToInt(z)};
			}
			inline Vec3i ceil() const {
				return {ceilToInt(x), ceilToInt(y), ceilToInt(z)};
			}
			inline Vec3i round() const {
				return {roundToInt(x), roundToInt(y), roundToInt(z)};
			}
			inline Vec3f step(const Vec3f& edge) const {
				return Vec3f((x < edge.x) ? 0.0f : 1.0f, (z < edge.z) ? 0.0f : 1.0f, (z < edge.z) ? 0.0f : 1.0f);
			}
			inline Vec3f clamped(float min, float max) const {
				return Vec3f(clamp(x, min, max), clamp(y, min, max), clamp(z, min, max));
			}
			inline Vec3f clamped(Vec3f min, Vec3f max) const {
				return Vec3f(clamp(x, min.x, max.x), clamp(y, min.y, max.y), clamp(z, min.z, max.z));
			}

			// Static
			inline static float dot(const Vec3f& l, const Vec3f& r) {
				return l.x * r.x + l.y * r.y + l.z * r.z;
			}
			inline static Vec3f cross(const Vec3f& l, const Vec3f& r) {
				return {r.y * l.z - r.z * l.y, r.z * l.x - r.x * l.z, r.x * l.y - r.y * l.x};
			}
			inline static Vec3f step(const Vec3f& in, const Vec3f& edge) {
				return Vec3f((in.x < edge.x) ? 0.0f : 1.0f, (in.z < edge.z) ? 0.0f : 1.0f, (in.z < edge.z) ? 0.0f : 1.0f);
			}
			inline static Vec3f lerp(const Vec3f& a, const Vec3f& b, float t) {
				return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)};
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
			static const Vec4f positiveInfinity;
			static const Vec4f negativeInfinity;
			static const Vec4f positiveInfinityPoint;
			static const Vec4f negativeInfinityPoint;
			static const Vec4f positiveInfinityDir;
			static const Vec4f negativeInfinityDir;
			static const Vec4f nan;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
			constexpr Vec4f(Vec3f xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
			constexpr Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
			constexpr Vec4f(__m128 xmm) : _xmm(xmm) {}

			/* ---- OPERATOR ---- */
			// Misc
			inline Vec4f operator-() const {
				return {-this->x, -this->y, -this->z, -this->w};
			}

			// Add/Sub
			inline Vec4f operator+(const Vec4f& other) const {
				return Vec4f(_mm_add_ps(_xmm, other._xmm));
			}
			inline Vec4f operator-(const Vec4f& other) const {
				return Vec4f(_mm_sub_ps(_xmm, other._xmm));
			}

			inline void operator+=(const Vec4f& other) {
				_xmm = _mm_add_ps(_xmm, other._xmm);
			}
			inline void operator-=(const Vec4f& other) {
				_xmm = _mm_sub_ps(_xmm, other._xmm);
			}

			// Mul/Div
			inline friend Vec4f operator*(const Vec4f& lhs, const Vec4f& rhs) {
				return Vec4f(_mm_mul_ps(lhs._xmm, rhs._xmm));
			}
			inline friend Vec4f operator*(const Vec4f& vec, float scalar) {
				return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
			}
			inline friend Vec4f operator*(float scalar, const Vec4f& vec) {
				return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
			}

			inline friend Vec4f operator/(const Vec4f& lhs, const Vec4f& rhs) {
				return Vec4f(_mm_div_ps(lhs._xmm, rhs._xmm));
			}
			inline friend Vec4f operator/(const Vec4f& vec, float scalar) {
				return Vec4f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
			}
			inline friend Vec4f operator/(float scalar, const Vec4f& vec) {
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
			inline bool operator==(const Vec4f& other) const {
				return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
			}
			inline bool operator!=(const Vec4f& other) const {
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
			inline float dot(const Vec4f& other) const {
				return x * other.x + y * other.y + z * other.z + w * other.w;
			}
			inline Vec4f normalized() const {
				return (*this) * invsqrt(this->dot(*this));
			}
			inline Vec4f normalized_precise() const {
				return (*this) / magnitude();
			}
			inline void normalize() {
				float inv = invsqrt(x * x + y * y + z * z + w * w);
				_mm_mul_ps(_xmm, _mm_set1_ps(inv));
			}
			inline void normalize_precise() {
				float mag = this->magnitude();
				_mm_div_ps(_xmm, _mm_set1_ps(mag));
			}
			inline float magnitude() const {
				return sqrt(x * x + y * y + z * z + w * w);
			}
			inline Vec4f abs() const {
				return Vec4f((x < 0.0f) ? -x : x, (y < 0.0f) ? -y : y, (z < 0.0f) ? -z : z, (w < 0.0f) ? -w : w);
			}
			inline Vec4f sign() const {
				return Vec4f((x < 0.0f) ? -1.0f : 1.0f, (y < 0.0f) ? -1.0f : 1.0f, (z < 0.0f) ? -1.0f : 1.0f, (w < 0.0f) ? -1.0f : 1.0f);
			}
			inline void inverse() {
				_mm_rcp_ps(_xmm);
			}
			inline Vec4f inversed() const {
				Vec4f r = *this;
				_mm_rcp_ps(r._xmm);
				return r;
			}
			inline Vec4i floor() const {
				return Vec4i(floorToInt(x), floorToInt(y), floorToInt(z), floorToInt(w));
			}
			inline Vec4i ceil() const {
				return Vec4i(ceilToInt(x), ceilToInt(y), ceilToInt(z), ceilToInt(w));
			}
			inline Vec4i round() const {
				return Vec4i(roundToInt(x), roundToInt(y), roundToInt(z), roundToInt(w));
			}
			inline Vec4f step(const Vec4f& edge) const {
				return step(*this, edge);
			}
			inline Vec4f clamped(float min, float max) const {
				return Vec4f(clamp(x, min, max), clamp(y, min, max), clamp(z, min, max), clamp(w, min, max));
			}
			inline Vec4f clamped(Vec4f min, Vec4f max) const {
				return Vec4f(clamp(x, min.x, max.x), clamp(y, min.y, max.y), clamp(z, min.z, max.z), clamp(w, min.w, max.w));
			}

			// Static
			inline static float dot(const Vec4f& l, const Vec4f& r) {
				return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
			}
			inline static Vec4f step(const Vec4f& in, const Vec4f& edge) {
				return Vec4f((in.x < edge.x) ? 0.0f : 1.0f, (in.y < edge.y) ? 0.0f : 1.0f, (in.z < edge.z) ? 0.0f : 1.0f, (in.w < edge.w) ? 0.0f : 1.0f);
			}
			inline static Vec4f lerp(const Vec4f& a, const Vec4f& b, float t) {
				return Vec4f(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t), lerpf(a.w, b.w, t));
			}
		};

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

			inline unsigned int toInt() const {
				unsigned int ir = (unsigned int) r * 255;
				unsigned int ib = (unsigned int) g * 255;
				unsigned int ig = (unsigned int) b * 255;
				unsigned int ia = (unsigned int) a * 255;
				return ir << 24 || ib << 16 || ig << 8 || ia;
			}

			inline void toBytesRGBA8(unsigned char* dest) const {
				dest[0] = (unsigned char) r * 255;
				dest[1] = (unsigned char) g * 255;
				dest[2] = (unsigned char) b * 255;
				dest[3] = (unsigned char) a * 255;
			}

		private:
			inline static float hsvIntermediate(float h, float s, float v, float n) {
				float k = fmodf(n + h / 60.0f, 6.0f);
				return v - v * s * maxf(minf(k, 4.0f - k, 1.0f), 0.0f);
			}
		};
	}
}
