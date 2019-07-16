#include "Perlin.h"

#include <cmath>
#include "XMath.h"



double perlind(double x, double y, double z) {
	int xi = floorToInt(x) & 255;
	int yi = floorToInt(y) & 255;
	int zi = floorToInt(z) & 255;

	x -= floorToInt(x);
	y -= floorToInt(y);
	z -= floorToInt(z);

	double u = faded(x);
	double v = faded(y);
	double w = faded(z);

	int A = perlinPermutationDoubled[xi] + yi;
	int AA = perlinPermutationDoubled[A] + zi;
	int AB = perlinPermutationDoubled[A + 1] + zi;
	int B = perlinPermutationDoubled[xi + 1] + yi;
	int BA = perlinPermutationDoubled[B] + zi;
	int BB = perlinPermutationDoubled[B + 1] + zi;

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

float perlinf(float x, float y, float z) {
	int xi = floorToInt(x) & 255;
	int yi = floorToInt(y) & 255;
	int zi = floorToInt(z) & 255;

	x -= floorToInt(x);
	y -= floorToInt(y);
	z -= floorToInt(z);

	float u = fadef(x);
	float v = fadef(y);
	float w = fadef(z);

	int A = perlinPermutationDoubled[xi] + yi;
	int AA = perlinPermutationDoubled[A] + zi;
	int AB = perlinPermutationDoubled[A + 1] + zi;
	int B = perlinPermutationDoubled[xi + 1] + yi;
	int BA = perlinPermutationDoubled[B] + zi;
	int BB = perlinPermutationDoubled[B + 1] + zi;

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

float perlinFBM(float x, float y, float z, int octaves, float lacunarity, float gain) {
	float noise = perlinf(x, y, z);
	float amp = gain;
	float freq = lacunarity;
	for (int i = 1; i < octaves; i++) {
		noise += perlinf(x * freq, y * freq, z * freq) * amp;
		amp *= gain;
		freq *= lacunarity;
	}
	return noise;
}
