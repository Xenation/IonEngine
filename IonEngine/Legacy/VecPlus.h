#pragma once

namespace IonEngine {
	inline namespace Math {

		/* ==== Vec2c ==== */
		struct Vec2c {
			union {
				struct {
					i8 x, y;
				};
				i8 data[2];
			};
		public:
			/* ---- CONSTANTS ---- */
			static const Vec2c zero;
			static const Vec2c one;
			static const Vec2c right;
			static const Vec2c left;
			static const Vec2c up;
			static const Vec2c down;
			static const Vec2c max;
			static const Vec2c min;

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec2c() : x(0), y(0) {}
			constexpr Vec2c(i8 x, i8 y) : x(x), y(y) {}

			/* ---- OPERATORS ---- */
			// Misc
			inline Vec2c operator-() {
				return Vec2c(-x, -y);
			}
			inline Vec2c operator+() {
				return Vec2c(+x, +y);
			}

			// Add/Sub
			inline Vec2c operator+(const Vec2c& o) const {
				return Vec2c(x + o.x, y + o.y);
			}
			inline Vec2c operator-(const Vec2c& o) const {
				return Vec2c(x - o.x, y + o.y);
			}
			inline void operator+=(const Vec2c& o) {
				x += o.x;
				y += o.y;
			}
			inline void operator-=(const Vec2c& o) {
				x -= o.x;
				y -= o.y;
			}

			// Mul/Div
			inline friend Vec2c operator*(const Vec2c& a, const Vec2c& b) {
				return Vec2c(a.x * b.x, a.y * b.y);
			}
			inline friend Vec2c operator*(const Vec2c& v, i8 m) {
				return Vec2c(v.x * m, v.y * m);
			}
			inline friend Vec2c operator*(i8 m, const Vec2c& v) {
				return Vec2c(v.x * m, v.y * m);
			}
			inline friend Vec2c operator/(const Vec2c& a, const Vec2c& b) {
				return Vec2c(a.x / b.x, a.y / b.y);
			}
			inline friend Vec2c operator/(const Vec2c& v, i8 d) {
				return Vec2c(v.x / d, v.y / d);
			}
			inline friend Vec2c operator/(i8 d, const Vec2c& v) {
				return Vec2c(v.x / d, v.y / d);
			}
			inline void operator*=(const Vec2c& o) {
				x *= o.x;
				y *= o.y;
			}
			inline void operator*=(i8 m) {
				x *= m;
				y *= m;
			}
			inline void operator/=(const Vec2c& o) {
				x /= o.x;
				y /= o.y;
			}
			inline void operator/=(i8 d) {
				x /= d;
				y /= d;
			}

			// Comp
			inline bool operator==(const Vec2c& o) const {
				return x == o.x && y == o.y;
			}
			inline bool operator!=(const Vec2c& o) const {
				return x != o.x || y != o.y;
			}

			// Index
			inline const i8& operator[](u32 index) const {
				return data[index];
			}
			inline const i8& operator[](i32 index) const {
				return data[index];
			}
			inline i8& operator[](u32 index) {
				return data[index];
			}
			inline i8& operator[](i32 index) {
				return data[index];
			}
		};
		/* ==== Vec2uc ==== */
		/* ==== Vec2s ==== */
		/* ==== Vec2us ==== */
		/* ==== Vec2ui ==== */
		/* ==== Vec2l ==== */
		/* ==== Vec2ul ==== */
		/* ==== Vec2d ==== */

		/* ==== Vec3c ==== */
		/* ==== Vec3uc ==== */
		/* ==== Vec3s ==== */
		/* ==== Vec3us ==== */
		/* ==== Vec3ui ==== */
		/* ==== Vec3l ==== */
		/* ==== Vec3ul ==== */
		/* ==== Vec3d ==== */

		/* ==== Vec4c ==== */
		/* ==== Vec4uc ==== */
		/* ==== Vec4s ==== */
		/* ==== Vec4us ==== */
		/* ==== Vec4ui ==== */
		/* ==== Vec4l ==== */
		/* ==== Vec4ul ==== */
		/* ==== Vec4d ==== */

	}
}
