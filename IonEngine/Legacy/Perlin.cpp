#include "Perlin.h"

#include <cmath>
#include "XMath.h"
using namespace IonEngine;



double IonEngine::perlind(double x, double y, double z) {
	i32 xi = floorToInt(x) & 255;
	i32 yi = floorToInt(y) & 255;
	i32 zi = floorToInt(z) & 255;

	x -= floorToInt(x);
	y -= floorToInt(y);
	z -= floorToInt(z);

	double u = faded(x);
	double v = faded(y);
	double w = faded(z);

	i32 A = perlinPermutationDoubled[xi] + yi;
	i32 AA = perlinPermutationDoubled[A] + zi;
	i32 AB = perlinPermutationDoubled[A + 1] + zi;
	i32 B = perlinPermutationDoubled[xi + 1] + yi;
	i32 BA = perlinPermutationDoubled[B] + zi;
	i32 BB = perlinPermutationDoubled[B + 1] + zi;

	return lerpd(
		lerpd(
			lerpd(
				gradd(perlinPermutationDoubled[AA], x, y, z),
				gradd(perlinPermutationDoubled[BA], x - 1, y, z), u),
			lerpd(
				gradd(perlinPermutationDoubled[AB], x, y - 1, z),
				gradd(perlinPermutationDoubled[BB], x - 1, y - 1, z), u), v),
		lerpd(
			lerpd(
				gradd(perlinPermutationDoubled[AA + 1], x, y, z - 1),
				gradd(perlinPermutationDoubled[BA + 1], x - 1, y, z - 1), u),
			lerpd(
				gradd(perlinPermutationDoubled[AB + 1], x, y - 1, z - 1),
				gradd(perlinPermutationDoubled[BB + 1], x - 1, y - 1, z - 1), u), v),
		w);
}

float IonEngine::perlinf(float x, float y, float z) {
	i32 xi = floorToInt(x) & 255;
	i32 yi = floorToInt(y) & 255;
	i32 zi = floorToInt(z) & 255;

	x -= floorToInt(x);
	y -= floorToInt(y);
	z -= floorToInt(z);

	float u = fadef(x);
	float v = fadef(y);
	float w = fadef(z);

	i32 A = perlinPermutationDoubled[xi] + yi;
	i32 AA = perlinPermutationDoubled[A] + zi;
	i32 AB = perlinPermutationDoubled[A + 1] + zi;
	i32 B = perlinPermutationDoubled[xi + 1] + yi;
	i32 BA = perlinPermutationDoubled[B] + zi;
	i32 BB = perlinPermutationDoubled[B + 1] + zi;

	return lerpf(
		lerpf(
			lerpf(
				gradf(perlinPermutationDoubled[AA], x, y, z),
				gradf(perlinPermutationDoubled[BA], x - 1, y, z), u),
			lerpf(
				gradf(perlinPermutationDoubled[AB], x, y - 1, z),
				gradf(perlinPermutationDoubled[BB], x - 1, y - 1, z), u), v),
		lerpf(
			lerpf(
				gradf(perlinPermutationDoubled[AA + 1], x, y, z - 1),
				gradf(perlinPermutationDoubled[BA + 1], x - 1, y, z - 1), u),
			lerpf(
				gradf(perlinPermutationDoubled[AB + 1], x, y - 1, z - 1),
				gradf(perlinPermutationDoubled[BB + 1], x - 1, y - 1, z - 1), u), v),
		w);
}

float IonEngine::perlinFBM(float x, float y, float z, u32 octaves, float lacunarity, float gain) {
	float noise = perlinf(x, y, z);
	float amp = gain;
	float freq = lacunarity;
	for (u32 i = 1; i < octaves; i++) {
		noise += perlinf(x * freq, y * freq, z * freq) * amp;
		amp *= gain;
		freq *= lacunarity;
	}
	return noise;
}
