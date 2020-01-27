#pragma once
#include <smmintrin.h> // SSE4.1
#include <cmath>
#include <limits>
#undef far
#undef near

namespace IonEngine {
	inline namespace Math {
		#pragma region Constants
		const __m128 _mm_one = _mm_set1_ps(1.0f);
		const __m128 _mm_negOne = _mm_set1_ps(-1.0f);
		const __m128 _mm_mask_signf = _mm_castsi128_ps(_mm_set1_epi32(0x80'00'00'00));

		namespace f32c {
			constexpr float pi = 3.1415926535897932384626433832795f;
			constexpr float two_pi = 6.283185307179586476925286766559f;
			constexpr float half_pi = 1.5707963267948966192313216916398f;
			constexpr float quarter_pi = 0.78539816339744830961566084581988f;
			constexpr float e = 2.7182818284590452353602874713527f;
			constexpr float deg2rad = 0.01745329251994329576923690768489f;
			constexpr float rad2deg = 57.295779513082320876798154814105f;
			constexpr float positiveInfinity = std::numeric_limits<float>::infinity();
			constexpr float negativeInfinity = -std::numeric_limits<float>::infinity();
			constexpr float nan = std::numeric_limits<float>::quiet_NaN();
		}

		namespace f64c {
			constexpr double pi = 3.1415926535897932384626433832795;
			constexpr double two_pi = 6.283185307179586476925286766559;
			constexpr double half_pi = 1.5707963267948966192313216916398;
			constexpr double quarter_pi = 0.78539816339744830961566084581988;
			constexpr double e = 2.7182818284590452353602874713527;
			constexpr double deg2rad = 0.01745329251994329576923690768489;
			constexpr double rad2deg = 57.295779513082320876798154814105;
			constexpr double positiveInfinity = std::numeric_limits<double>::infinity();
			constexpr double negativeInfinity = -std::numeric_limits<double>::infinity();
			constexpr double nan = -std::numeric_limits<double>::quiet_NaN();
		}

		#pragma endregion

		#pragma region MathFunctions
		inline int maxi(int a, int b) {
			return (a > b) ? a : b;
		}
		inline int maxi(int a, int b, int c) {
			return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
		}

		inline int mini(int a, int b) {
			return (a < b) ? a : b;
		}
		inline int mini(int a, int b, int c) {
			return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
		}

		inline float maxf(float a, float b) {
			return (a > b) ? a : b;
		}
		inline float maxf(float a, float b, float c) {
			return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
		}

		inline float minf(float a, float b) {
			return (a < b) ? a : b;
		}
		inline float minf(float a, float b, float c) {
			return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
		}

		inline void minmax3f(float a, float b, float c, float& min, float& max) {
			__m128 ma = _mm_load_ss(&a);
			__m128 mb = _mm_load_ss(&b);
			__m128 mc = _mm_load_ss(&c);
			_mm_store_ss(&max, _mm_max_ss(_mm_max_ss(ma, mb), mc));
			_mm_store_ss(&max, _mm_min_ss(_mm_min_ss(ma, mb), mc));
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

		inline float sqrt(float n) {
			float r;
			_mm_store_ss(&r, _mm_sqrt_ss(_mm_load_ss(&n)));
			return r;
		}

		inline float invsqrt(float n) {
			float r;
			_mm_store_ss(&r, _mm_rsqrt_ss(_mm_load_ss(&n)));
			return r;
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

		inline int roundToInt(double d) {
			return (d > 0) ? (int) (d + 0.5f) : (int) (d - 0.5f);
		}

		inline int roundToInt(float f) {
			return (f > 0) ? (int) (f + 0.5f) : (int) (f - 0.5f);
		}

		inline float clamp(float f, float min, float max) {
			return (f < min) ? min : ((f > max) ? max : f);
		}

		inline int clamp(int i, int min, int max) {
			return (i < min) ? min : ((i > max) ? max : i);
		}

		inline void swapToMinMax(int& min, int& max) {
			if (max < min) {
				int tmp = min;
				min = max;
				max = tmp;
			}
		}
		inline void swapToMinMax(float& min, float& max) {
			if (max < min) {
				float tmp = min;
				min = max;
				max = tmp;
			}
		}
		#pragma endregion
	}
}
