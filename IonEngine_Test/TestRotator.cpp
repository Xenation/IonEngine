#include "TestRotator.h"

#include <Transform.h>
#include <Time.h>



TestRotator::TestRotator(Entity* entity) : Component(entity) {}

TestRotator::~TestRotator() {}


void TestRotator::onUpdate() {
	float dr = ((float) M_PI_2) * Time::deltaTime;
	//transform->rotate(Rotor3f(transform->forward(), Vec3f(0, sin(dr), cos(dr))));
	transform->rotate(Rotor3f::euler({ ((float) M_PI_2) * Time::deltaTime, 0, 0 }));
}
