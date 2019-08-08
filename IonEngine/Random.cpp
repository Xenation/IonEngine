#include "Random.h"

using namespace IonEngine;



Random* Random::i = new Random(666);


Random::Random(unsigned int seed) : seed(seed), mersenneTwister(seed) {}
Random::Random(std::string seed) : seed(0) {
	std::seed_seq(seed.begin(), seed.end()).generate((unsigned int*) this, ((unsigned int*) this) + 1);
	mersenneTwister = std::mt19937(this->seed);
}

Random::~Random() {}


float Random::uniform() {
	return mersenneTwister() / (float) UINT_MAX;
}

float Random::range(float min, float max) {
	float m = (max - min) / (float) UINT_MAX;
	return mersenneTwister() * m + min;
}

int Random::range(int min, int max) {
	return floorToInt(range((float) min, (float) max));
}

Vec2f Random::inCircle(float radius) {
	float a = uniform() * two_pi;
	float r = radius * sqrtfInline(uniform());
	return Vec2f(r * cosf(a), r * sinf(a));
}

Vec3f Random::inSphere(float radius) {
	float u = 2.0f * uniform() - 1.0f;
	float phi = two_pi * uniform();
	float m = sqrtfInline(1.0f - u * u);
	float r = cbrt(uniform());
	return Vec3f(cosf(phi) * m * r, sinf(phi) * m * r, u);
}

Vec2f Random::onCircle(float radius) {
	float a = uniform() * two_pi;
	return Vec2f(radius * cosf(a), radius * sinf(a));
}

Vec3f Random::onSphere(float radius) {
	float u = 2.0f * uniform() - 1.0f;
	float phi = two_pi * uniform();
	float m = sqrtfInline(1.0f - u * u);
	return Vec3f(cosf(phi) * m * radius, sinf(phi) * m * radius, u);
}

Color Random::color(float hMin, float hMax, float sMin, float sMax, float vMin, float vMax, float aMin, float aMax) {
	return Color::fromHSV(range(hMin, hMax), range(sMin, sMax), range(vMin, vMax), range(aMin, aMax));
}