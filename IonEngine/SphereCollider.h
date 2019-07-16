#pragma once
#include "Collider.h"

class SphereCollider : public Collider {
public:
	SphereCollider(float radius);
	~SphereCollider();

	inline float getRadius() const { return radius; }

private:
	float radius;
};

