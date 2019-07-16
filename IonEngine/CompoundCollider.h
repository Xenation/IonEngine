#pragma once
#include "Collider.h"
#include "XMath.h"

class CompoundCollider : public Collider {
public:
	CompoundCollider();
	~CompoundCollider();

	void addCollider(Collider* collider, Vec3f position, Quaternion rotation);
	void removeCollider(Collider* collider);
};

