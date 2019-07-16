#include "TestRotator.h"

#include <Transform.h>
#include <Time.h>



TestRotator::TestRotator(Entity* entity) : Component(entity) {}

TestRotator::~TestRotator() {}


void TestRotator::onUpdate() {
	transform->rotate(Quaternion::euler({ ((float) M_PI_2) * Time::deltaTime, 0, 0 }));
}
