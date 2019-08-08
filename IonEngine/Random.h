#pragma once
#include <string>
#include <random>
#include "XMath.h"

namespace IonEngine {
	class Random {
	public:
		/// <value>A default instance of Random for ease of use</value>
		static Random* i;

		/// <value>The seed of this Random Number Generator</value>
		const int seed;

		/// <summary>
		/// Creates a Random Number Generator with the given seed
		/// </summary>
		Random(unsigned int seed);
		/// <summary>
		/// Creates a Random Number Generator with the given seed
		/// </summary>
		Random(std::string seed);
		~Random();

		/// <summary>
		/// Returns a random float in the range [0,1]
		/// </summary>
		float uniform();
		/// <summary>
		/// Returns a random float between min (included) and max (included)
		/// </summary>
		float range(float min, float max);
		/// <summary>
		/// Returns a random int between min (included) and max (excluded)
		/// </summary>
		int range(int min, int max);
		/// <summary>
		/// Returns a random 2D point inside a circle of the given radius
		/// </summary>
		Vec2f inCircle(float radius = 1.0f);
		/// <summary>
		/// Returns a random 3D point inside a sphere of the given radius
		/// </summary>
		Vec3f inSphere(float radius = 1.0f);
		/// <summary>
		/// Returns a random 2D point on a circle of the given radius
		/// </summary>
		Vec2f onCircle(float radius = 1.0f);
		/// <summary>
		/// Returns a random 3D point on a sphere of the given radius
		/// </summary>
		Vec3f onSphere(float radius = 1.0f);
		/// <summary>
		/// Returns a random color, optionally using the given HSV bounds
		/// </summary>
		Color color(float hMin = 0.0f, float hMax = 360.0f, float sMin = 0.5f, float sMax = 1.0f, float vMin = 0.5f, float vMax = 1.0f, float aMin = 1.0f, float aMax = 1.0f);

	private:
		std::mt19937 mersenneTwister;
	};
}
