#pragma once

namespace IonEngine {
	inline namespace Math {

		template<typename T>
		struct Vec2 {
			union {
				struct {
					T x, y;
				};
				T data[2];
			};
		public:

			/* ---- CONSTRUCTORS ---- */
			constexpr Vec2() : x(), y() {}
			constexpr Vec2(T x, T y) : x(x), y(y) {}

			/* ---- OPERATORS ---- */
			// Misc
			inline Vec2<T> operator-() {
				return Vec2<T>(-x, -y);
			}
			inline Vec2<T> operator+() {
				return Vec2<T>(+x, +y);
			}

			// Add/Sub
			inline Vec2<T> operator+(const Vec2& o) const {
				return Vec2<T>(x + o.x, y + o.y);
			}
			inline Vec2<T> operator-(const Vec2<T>& o) const {
				return Vec2<T>(x - o.x, y + o.y);
			}
			inline void operator+=(const Vec2<T>& o) {
				x += o.x;
				y += o.y;
			}
			inline void operator-=(const Vec2<T>& o) {
				x -= o.x;
				y -= o.y;
			}

			// Mul/Div
			inline friend Vec2<T> operator*(const Vec2<T>& a, const Vec2<T>& b) {
				return Vec2<T>(a.x * b.x, a.y * b.y);
			}
			inline friend Vec2<T> operator*(const Vec2<T>& v, T m) {
				return Vec2(v.x * m, v.y * m);
			}
			inline friend Vec2<T> operator*(T m, const Vec2<T>& v) {
				return Vec2<T>(v.x * m, v.y * m);
			}
			inline friend Vec2<T> operator/(const Vec2<T>& a, const Vec2<T>& b) {
				return Vec2<T>(a.x / b.x, a.y / b.y);
			}
			inline friend Vec2<T> operator/(const Vec2<T>& v, T d) {
				return Vec2<T>(v.x / d, v.y / d);
			}
			inline friend Vec2<T> operator/(T d, const Vec2<T>& v) {
				return Vec2<T>(v.x / d, v.y / d);
			}
			inline void operator*=(const Vec2<T>& o) {
				x *= o.x;
				y *= o.y;
			}
			inline void operator*=(T m) {
				x *= m;
				y *= m;
			}
			inline void operator/=(const Vec2<T>& o) {
				x /= o.x;
				y /= o.y;
			}
			inline void operator/=(T d) {
				x /= d;
				y /= d;
			}

			// Comp
			inline bool operator==(const Vec2<T>& o) const {
				return x == o.x && y == o.y;
			}
			inline bool operator!=(const Vec2<T>& o) const {
				return x != o.x || y != o.y;
			}

			// Index
			inline const T& operator[](u32 index) const {
				return data[index];
			}
			inline const T& operator[](i32 index) const {
				return data[index];
			}
			inline T& operator[](u32 index) {
				return data[index];
			}
			inline T& operator[](i32 index) {
				return data[index];
			}

			/* ---- METHODS ---- */
			inline T dot(const Vec2<T>& o) const {
				return x * o.x + y * o.y;
			}
			inline float magnitude() const {
				return sqrtfOpt((float) (x * x + y * y));
			}
			inline static int dot(const Vec2<T>& l, const Vec2<T>& r) {
				return l.x * r.x + l.y * r.y;
			}
			// Copy Operations
			inline Vec2<T> abs() const {
				return Vec2<T>((x < 0) ? -x : x, (y < 0) ? -y : y);
			}
			inline Vec2<T> sign() const {
				return Vec2<T>((x < 0) ? -1 : 1, (y < 0) ? -1 : 1);
			}
			inline Vec2<T> step(const Vec2<T>& edge) const {
				return Vec2<T>((x < edge.x) ? 0 : 1, (y < edge.y) ? 0 : 1);
			}
			inline Vec2<T> clamped(T min, T max) const {
				return Vec2<T>(clamp(x, min, max), clamp(y, min, max));
			}
			inline Vec2<T> clamped(const Vec2<T>& min, const Vec2<T>& max) const {
				return Vec2<T>(clamp(x, min.x, max.x), clamp(y, min.y, max.y));
			}
			inline Vec2<T> normalized() const {
				
			}
			inline static Vec2<T> step(const Vec2<T> in, const Vec2<T>& edge) {
				return Vec2<T>((in.x < edge.x) ? 0 : 1, (in.y < edge.y) ? 0 : 1);
			}
			// Modify Operations
			inline void abs() {

			}
		};

		template<>
		struct Vec2<i8> {
			static const Vec2<i8> zero;
			static const Vec2<i8> one;
			static const Vec2<i8> right;
			static const Vec2<i8> left;
			static const Vec2<i8> up;
			static const Vec2<i8> down;
			static const Vec2<i8> max;
			static const Vec2<i8> min;
		};

		template<typename T>
		struct Vec3 {
			union {
				struct {
					T x, y, z;
				};
				struct {
					Vec2<T> xy;
					T __z;
				};
				T data[3];
			};
		};

		template<typename T>
		struct Vec4 {
			union {
				struct {
					T x, y, z, w;
				};
				struct {
					Vec3<T> xyz;
					T __w;
				};
				T data[4];
			};
		};

		// Vec2
		typedef Vec2<i8> Vec2c;
		typedef Vec2<u8> Vec2uc;
		typedef Vec2<i16> Vec2s;
		typedef Vec2<u16> Vec2us;
		typedef Vec2<i32> Vec2i;
		typedef Vec2<u32> Vec2ui;
		typedef Vec2<i64> Vec2l;
		typedef Vec2<u64> Vec2ul;
		typedef Vec2<f32> Vec2f;
		typedef Vec2<f64> Vec2d;

		// Vec3
		typedef Vec3<i8> Vec3c;
		typedef Vec3<u8> Vec3uc;
		typedef Vec3<i16> Vec3s;
		typedef Vec3<u16> Vec3us;
		typedef Vec3<i32> Vec3i;
		typedef Vec3<u32> Vec3ui;
		typedef Vec3<i64> Vec3l;
		typedef Vec3<u64> Vec3ul;
		typedef Vec3<f32> Vec3f;
		typedef Vec3<f64> Vec3d;

		// Vec4
		typedef Vec4<i8> Vec4c;
		typedef Vec4<u8> Vec4uc;
		typedef Vec4<i16> Vec4s;
		typedef Vec4<u16> Vec4us;
		typedef Vec4<i32> Vec4i;
		typedef Vec4<u32> Vec4ui;
		typedef Vec4<i64> Vec4l;
		typedef Vec4<u64> Vec4ul;
		typedef Vec4<f32> Vec4f;
		typedef Vec4<f64> Vec4d;

	}
}
