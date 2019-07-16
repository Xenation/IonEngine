#pragma once
#include "Collider.h"
#include "XMath.h"

class BoxCollider : public Collider {
public:
	BoxCollider(Vec3f halfExtents);
	~BoxCollider();

	inline Vec3f getHalfExtents() const { return halfExtents; }

protected:
	Vec3f halfExtents;
};

