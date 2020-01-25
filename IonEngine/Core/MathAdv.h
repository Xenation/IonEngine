#pragma once
#include "MathVec.h"

namespace IonEngine {
	inline namespace Math {
		/* ==== BIVEC3f ==== */
		struct Bivec3f {
			float xy, xz, yz;

		public:
			inline Bivec3f() : xy(0.0f), xz(0.0f), yz(0.0f) {}
			inline Bivec3f(float xy, float xz, float yz) : xy(xy), xz(xz), yz(yz) {}

			inline static Bivec3f wedge(const Vec3f& u, const Vec3f& v) {
				return {
					u.x * v.y - u.y * v.x,	// XY
					u.x * v.z - u.z * v.x,	// XZ
					u.y * v.z - u.z * v.y	// YZ
				};
			}
		};

		/* ==== ROTOR3f ==== */
		struct Rotor3f {
			union {
				struct {
					float a;
					union {
						Bivec3f bv;
						struct {
							float xy, xz, yz;
						};
					};
				};
				float data[4];
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
			inline friend bool operator!=(const Rotor3f& a, const Rotor3f& b) {
				// Does not take in account equivalent but opposite rotations
				return a.a != b.a || a.xy != b.xy || a.xz != b.xz || a.yz != b.yz;
			}
			inline friend bool operator==(const Rotor3f& a, const Rotor3f& b) {
				// Does not take in account equivalent but opposite rotations
				return !(a != b);
			}

			/* ----- METHODS ---- */
			inline static Rotor3f AnglePlane(float angleRad, const Bivec3f& bvPlane) {
				float sina = std::sin(angleRad / 2.f);
				return {std::cos(angleRad / 2.f), {Bivec3f(-sina * bvPlane.xy, -sina * bvPlane.xz, -sina * bvPlane.yz)}};
			}
			inline static Rotor3f euler(const Vec3f& eulerAngles) {
				return attitude(eulerAngles.y, eulerAngles.x, eulerAngles.z);
			}
			inline static Rotor3f euler(float eulerX, float eulerY, float eulerZ) {
				return attitude(eulerY, eulerX, eulerZ);
			}
			inline static Rotor3f attitude(float yaw, float pitch, float roll) {
				// TODO optimize
				//Rotor3f ry;
				//ry.a = cos(yaw * 0.5f);
				//ry.xz = sin(yaw * 0.5f);

				//Rotor3f rp;
				//rp.a = cos(pitch * 0.5f);
				//rp.yz = -sin(pitch * 0.5f);

				//Rotor3f rr;
				//rr.a = cos(roll * 0.5f);
				//rr.xy = -sin(roll * 0.5f);

				float cosYaw = std::cos(yaw * 0.5f);
				float sinYaw = std::sin(yaw * 0.5f);
				float cosPitch = std::cos(pitch * 0.5f);
				float sinPitch = std::sin(pitch * 0.5f);
				float cosRoll = std::cos(roll * 0.5f);
				float sinRoll = std::sin(roll * 0.5f);

				Rotor3f ryp;
				ryp.a = cosYaw * cosPitch;
				ryp.xy = sinYaw * sinPitch;
				ryp.xz = sinYaw * cosPitch;
				ryp.yz = cosYaw * -sinPitch;

				Rotor3f rypr;
				rypr.a = ryp.a * cosRoll - ryp.xy * -sinRoll;
				rypr.xy = ryp.xy * cosRoll + ryp.a * -sinRoll;
				rypr.xz = ryp.xz * cosRoll - ryp.yz * -sinRoll;
				rypr.yz = ryp.yz * cosRoll + ryp.xz * -sinRoll;

				//Debug::log("ROTOR", std::string("input: y" + std::to_string(yaw) + " p" + std::to_string(pitch) + " r" + std::to_string(roll)));
				//Debug::log("ROTOR", std::string("possible: ry:" + ry.toString()));
				//Debug::log("ROTOR", std::string("possible: rp:" + rp.toString()));
				//Debug::log("ROTOR", std::string("possible: rr:" + rr.toString()));
				return rypr;
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
				return sqrt(magnitudeSqr());
			}
			inline void normalize() {
				float isqrt = invsqrt(a * a + xy * xy + xz * xz + yz * yz);
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

		/* ===== MATRIX4x4f ==== */
		struct Matrix4x4f {
			union {
				// Stored in column major order but accessed in row major (first digit is row/y, second is column/x)
				// Transposed in row major it would look like this
				//		c0	c1	c2	c3
				// r0	m00	m01	m02	m03
				// r1	m10	m11	m12	m13
				// r2	m20	m21	m22	m23
				// r3	m30	m31	m32	m33
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
				m00(0.0f), m10(0.0f), m20(0.0f), m30(0.0f),
				m01(0.0f), m11(0.0f), m21(0.0f), m31(0.0f),
				m02(0.0f), m12(0.0f), m22(0.0f), m32(0.0f),
				m03(0.0f), m13(0.0f), m23(0.0f), m33(0.0f) {}
			constexpr Matrix4x4f(float m00, float m10, float m20, float m30, float m01, float m11, float m21, float m31, float m02, float m12, float m22, float m32, float m03, float m13, float m23, float m33) :
				m00(m00), m10(m10), m20(m20), m30(m30),
				m01(m01), m11(m11), m21(m21), m31(m31),
				m02(m02), m12(m12), m22(m22), m32(m32),
				m03(m03), m13(m13), m23(m23), m33(m33) {}

			/* ---- OPERATORS ---- */
			inline static void transfColumn(const Matrix4x4f* mat, const __m128& column, __m128& res) {
				res = _mm_mul_ps(mat->_xmm[0], _mm_set1_ps(column.m128_f32[0]));
				res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[1], _mm_set1_ps(column.m128_f32[1])));
				res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[2], _mm_set1_ps(column.m128_f32[2])));
				res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[3], _mm_set1_ps(column.m128_f32[3])));
			}
			inline Matrix4x4f operator*(const Matrix4x4f& other) const {
				Matrix4x4f m;
				transfColumn(this, other._xmm[0], m._xmm[0]);
				transfColumn(this, other._xmm[1], m._xmm[1]);
				transfColumn(this, other._xmm[2], m._xmm[2]);
				transfColumn(this, other._xmm[3], m._xmm[3]);
				return m;
			}
			inline Vec4f operator*(const Vec4f& other) const {
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
			void luDecomposition(Matrix4x4f& lu);
			static Vec4f solve(Matrix4x4f& lu, Vec4f toSolve);

			inline Matrix4x4f transposed() const {
				return Matrix4x4f(
					m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33
				);
			}
			inline void transpose() {
				_MM_TRANSPOSE4_PS(_xmm[0], _xmm[1], _xmm[2], _xmm[3]);
			}

			// Access
			inline float get(Vec2i pos) {
				return columns[pos.x][pos.y];
			}
			inline void set(Vec2i pos, float val) {
				columns[pos.x][pos.y] = val;
			}
			inline float& at(int row, int col) {
				return columns[col][row];
			}
			inline Vec3f& translation() {
				return columns[3].xyz;
			}
			inline Vec3f scale() {
				return Vec3f(columns[0].xyz.magnitude(), columns[1].xyz.magnitude(), columns[2].xyz.magnitude());
			}
			inline Rotor3f rotor() {
				// Extract pure rotation matrix
				Vec3f sca = scale();
				Matrix4x4f rotMat = Matrix4x4f(
					m00, m10, m20, 0.0f,
					m01, m11, m21, 0.0f,
					m02, m12, m22, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				rotMat.columns[0] /= sca.x;
				rotMat.columns[1] /= sca.y;
				rotMat.columns[2] /= sca.z;

				// Unrotated frame
				// x: Vec3f::right
				// y: Vec3f::up
				// z: Vec3f::forward

				// Rotated frame
				const Vec3f& xr = rotMat.columns[0].xyz;
				const Vec3f& yr = rotMat.columns[1].xyz;
				const Vec3f& zr = rotMat.columns[2].xyz;

				Rotor3f r = Rotor3f(-(1 + xr.x + yr.y + zr.z), xr.y - yr.x, xr.z - zr.x, yr.z - zr.y); // minus sign needed for some reason
				r.normalize();
				return r;
			}

			// Write
			inline void setRotationScale(Rotor3f rot, Vec3f scale) {
				m00 = rot.a * rot.a - rot.xy * rot.xy - rot.xz * rot.xz + rot.yz * rot.yz; m10 = -2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz; m20 = -2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy;
				m01 = 2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz; m11 = rot.a * rot.a - rot.xy * rot.xy + rot.xz * rot.xz - rot.yz * rot.yz; m21 = -2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy;
				m02 = 2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy; m12 = 2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy; m22 = rot.a * rot.a + rot.xy * rot.xy - rot.xz * rot.xz - rot.yz * rot.yz;

				_mm_mul_ps(_xmm[0], _mm_set1_ps(scale.x));
				_mm_mul_ps(_xmm[1], _mm_set1_ps(scale.y));
				_mm_mul_ps(_xmm[2], _mm_set1_ps(scale.z));
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
			inline Matrix4x4f inversePerspective() const {
				return Matrix4x4f(
					1.0f / m00, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f / m11, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f / m23,
					0.0f, 0.0f, 1.0f, m22 / -m23
				);
			}
			inline Vec3f multPoint(const Vec3f& point) const {
				__m128 vreg;
				vreg = _mm_mul_ps(_xmm[0], _mm_set1_ps(point.x));
				vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[1], _mm_set1_ps(point.y)));
				vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[2], _mm_set1_ps(point.z)));
				vreg = _mm_add_ps(vreg, _xmm[3]);
				return Vec3f(vreg.m128_f32[0], vreg.m128_f32[1], vreg.m128_f32[2]);
			}
			inline Vec3f multDirection(const Vec3f& dir) const {
				Vec3f v;
				__m128 vreg;
				vreg = _mm_mul_ps(_xmm[0], _mm_set1_ps(dir.x));
				vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[1], _mm_set1_ps(dir.y)));
				vreg = _mm_add_ps(vreg, _mm_mul_ps(_xmm[2], _mm_set1_ps(dir.z)));
				return Vec3f(vreg.m128_f32[0], vreg.m128_f32[1], vreg.m128_f32[2]);
			}

			// Transformations
			static Matrix4x4f translation(const Vec3f& transVec) {
				return Matrix4x4f(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					transVec.x, transVec.y, transVec.z, 1
				);
			}
			static Matrix4x4f scale(const Vec3f& scaleVec) {
				return Matrix4x4f(
					scaleVec.x, 0.0f, 0.0f, 0.0f,
					0.0f, scaleVec.y, 0.0f, 0.0f,
					0.0f, 0.0f, scaleVec.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);
			}
			static Matrix4x4f rotation(const Rotor3f& rot) {
				return Matrix4x4f(
					rot.a * rot.a - rot.xy * rot.xy - rot.xz * rot.xz + rot.yz * rot.yz, -2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz, -2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy, 0.0f,
					2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz, rot.a * rot.a - rot.xy * rot.xy + rot.xz * rot.xz - rot.yz * rot.yz, -2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy, 0.0f,
					2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy, 2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy, rot.a * rot.a + rot.xy * rot.xy - rot.xz * rot.xz - rot.yz * rot.yz, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);
			}
			static Matrix4x4f translationScale(const Vec3f& transVec, const Vec3f& scaleVec) {
				return Matrix4x4f(
					scaleVec.x, 0.0f, 0.0f, 0.0f,
					0.0f, scaleVec.y, 0.0f, 0.0f,
					0.0f, 0.0f, scaleVec.z, 0.0f,
					transVec.x, transVec.y, transVec.z, 1.0f
				);
			}
			static Matrix4x4f translationRotation(const Vec3f& transVec, const Rotor3f& rot) {
				return Matrix4x4f(
					rot.a * rot.a - rot.xy * rot.xy - rot.xz * rot.xz + rot.yz * rot.yz, -2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz, -2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy, 0.0f,
					2.0f * rot.a * rot.xy - 2.0f * rot.yz * rot.xz, rot.a * rot.a - rot.xy * rot.xy + rot.xz * rot.xz - rot.yz * rot.yz, -2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy, 0.0f,
					2.0f * rot.a * rot.xz + 2.0f * rot.yz * rot.xy, 2.0f * rot.a * rot.yz - 2.0f * rot.xz * rot.xy, rot.a * rot.a + rot.xy * rot.xy - rot.xz * rot.xz - rot.yz * rot.yz, 0.0f,
					transVec.x, transVec.y, transVec.z, 1.0f
				);
			}
			static Matrix4x4f transformation(const Vec3f& translationVec, const Vec3f& scaleVec, const Rotor3f& rotor) {
				return translationRotation(translationVec, rotor) * scale(scaleVec);
			}
			static Matrix4x4f perspectiveProjection(float degFov, float aspect, float nearPlane, float farPlane) {
				float fov = degFov * deg2rad;
				float zRange = farPlane - nearPlane;

				float xScale = 1.0f / tanf(fov * 0.5f);
				float yScale = xScale * aspect;

				return Matrix4x4f(
					xScale, 0.0f, 0.0f, 0.0f,
					0.0f, yScale, 0.0f, 0.0f,
					0.0f, 0.0f, (farPlane + nearPlane) / zRange, 1.0f,
					0.0f, 0.0f, -(2.0f * farPlane * nearPlane) / zRange, 0.0f
				);
			}
			static Matrix4x4f orthogonalProjection(float right, float left, float top, float bottom, float near, float far) {
				float horizRange = right - left;
				float vertiRange = top - bottom;
				float depthRange = far - near;
				return Matrix4x4f(
					2.0f / horizRange, 0.0f, 0.0f, 0.0f,
					0.0f, 2.0f / vertiRange, 0.0f, 0.0f,
					0.0f, 0.0f, 2.0f / depthRange, 0.0f,
					-(right + left) / horizRange, -(top + bottom) / vertiRange, -(far + near) / depthRange, 1.0f
				);
			}

			// Conversions
			inline Vec3f eulerRotation() {
				Vec3f t;
				if (m12 < 1.0f) {
					if (m12 > -1.0f) {
						t.x = std::asin(-m12);
						t.y = std::atan2(m02, m22);
						t.z = std::atan2(m10, m11);
					} else {
						t.x = half_pi;
						t.y = -std::atan2(-m01, m00);
						t.z = 0.0f;
					}
				} else {
					t.x = -half_pi;
					t.y = std::atan2(-m01, m00);
					t.z = 0.0f;
				}
				return t;
			}
		};
	}
}
