#pragma once

#include "bullet/btBulletDynamicsCommon.h"
#include "Component.h"

class Transform;
class Entity;
class Collider;

class SynchronizedTransform : public btMotionState {
public:
	Transform* transform;

	SynchronizedTransform(Transform* transform);
	~SynchronizedTransform();

	void getWorldTransform(btTransform& worldTransform) const override;
	void setWorldTransform(const btTransform& worldTransform) override;
};

class Rigidbody : public Component {
public:

	Rigidbody(Entity* entity);
	Rigidbody(Entity* entity, Collider* collider);
	~Rigidbody();
	
	void setMass(float mass);
	inline float getMass() { return mass; }
	void setKinematic(bool kinematic);
	void setCollider(Collider* collider);
	inline Collider* getCollider() { return collider; }

	virtual void onEnable() override;
	virtual void onDisable() override;

	inline btRigidBody* getBulletBody() { return body; }

protected:
	float mass = 0.0f;
	Collider* collider = nullptr;
	SynchronizedTransform syncTransform;
	btRigidBody* body;
};

