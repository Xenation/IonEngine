#pragma once

class btCollisionShape;
class CompoundCollider;

class Collider {
public:
	static Collider*const empty;

	CompoundCollider* _parent = nullptr; // TODO ugly and does not allow multiple compound to use a same shape

	Collider();
	Collider(btCollisionShape* shape);
	~Collider();

	inline btCollisionShape* getBulletShape() const { return shape; }

protected:
	btCollisionShape* shape = nullptr;
};

