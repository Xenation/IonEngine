#pragma once

#include <smmintrin.h>

namespace IonEngine {
	inline namespace Math {
		
		/* ==== Instruction Ops ==== */
		// Sqrt
		inline float sqrt(float x) {
			__m128 r = _mm_sqrt_ss(_mm_set_ss(x));
			return *reinterpret_cast<float*>(&r);
		}
		inline double sqrt(double x) {
			__m128d r = _mm_sqrt_sd(_mm_set_sd(0.0), _mm_set_sd(x));
			return *reinterpret_cast<double*>(&r);
		}
		// Min
		inline i8 min(i8 a, i8 b) {
			return (a < b) ? a : b;
		}
		inline u8 min(u8 a, u8 b) {
			return (a < b) ? a : b;
		}
		inline i16 min(i16 a, i16 b) {
			return (a < b) ? a : b;
		}
		inline u16 min(u16 a, u16 b) {
			return (a < b) ? a : b;
		}
		inline i32 min(i32 a, i32 b) {
			return (a < b) ? a : b;
		}
		inline u32 min(u32 a, u32 b) {
			return (a < b) ? a : b;
		}
		inline i64 min(i64 a, i64 b) {
			return (a < b) ? a : b;
		}
		inline u64 min(u64 a, u64 b) {
			return (a < b) ? a : b;
		}
		inline float min(float a, float b) {
			return (a < b) ? a : b;
		}
		inline double min(double a, double b) {
			return (a < b) ? a : b;
		}
		// Max
		inline i8 max(i8 a, i8 b) {
			return (a > b) ? a : b;
		}
		inline u8 max(u8 a, u8 b) {
			return (a > b) ? a : b;
		}
		inline i16 max(i16 a, i16 b) {
			return (a > b) ? a : b;
		}
		inline u16 max(u16 a, u16 b) {
			return (a > b) ? a : b;
		}
		inline i32 max(i32 a, i32 b) {
			return (a > b) ? a : b;
		}
		inline u32 max(u32 a, u32 b) {
			return (a > b) ? a : b;
		}
		inline i64 max(i64 a, i64 b) {
			return (a > b) ? a : b;
		}
		inline u64 max(u64 a, u64 b) {
			return (a > b) ? a : b;
		}
		inline float max(float a, float b) {
			return (a > b) ? a : b;
		}
		// Clamp
		inline i8 clamp(i8 x, i8 min, i8 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline u8 clamp(u8 x, u8 min, u8 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline i16 clamp(i16 x, i16 min, i16 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline u16 clamp(u16 x, u16 min, u16 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline i32 clamp(i32 x, i32 min, i32 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline u32 clamp(u32 x, u32 min, u32 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline i64 clamp(i64 x, i64 min, i64 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline u64 clamp(u64 x, u64 min, u64 max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline float clamp(float x, float min, float max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
		inline double clamp(double x, double min, double max) {
			return (x > min) ? ((x < max) ? x : max) : min;
		}
	}
}

#ifdef ION_MATH_USE_TEMPLATES
#include "VecTmpl.h"
#else
#include "Vec.h"
#ifdef ION_MATH_VEC_PLUS
#include "VecPlus.h"
#endif
#endif
