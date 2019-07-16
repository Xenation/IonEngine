#include "Rigidbody.h"

#include "Transform.h"
#include "Entity.h"
#include "Engine.h"
#include "PhysicsWorld.h"
#include "Collider.h"



SynchronizedTransform::SynchronizedTransform(Transform* transform) : transform(transform) {
	
}

SynchronizedTransform::~SynchronizedTransform() {

}


void SynchronizedTransform::getWorldTransform(btTransform& worldTransform) const {
	Vec3f pos = transform->getWorldPosition();
	Quaternion rot = transform->getWorldRotation();
	worldTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
	worldTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
}

void SynchronizedTransform::setWorldTransform(const btTransform& worldTransform) {
	btVector3 pos = worldTransform.getOrigin();
	btQuaternion rot = worldTransform.getRotation();
	transform->setWorldPosition(Vec3f(pos.x(), pos.y(), pos.z()));
	transform->setWorldRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
}



Rigidbody::Rigidbody(Entity* entity) : Component(entity), syncTransform(entity->transform) {
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, &syncTransform, Collider::empty->getBulletShape());
	body = new btRigidBody(rbInfo);
}

Rigidbody::Rigidbody(Entity* entity, Collider* collider) : Component(entity), syncTransform(entity->transform), collider(collider) {
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, &syncTransform, collider->getBulletShape());
	body = new btRigidBody(rbInfo);
}

Rigidbody::~Rigidbody() {
	if (body != nullptr) {
		delete body;
	}
}


void Rigidbody::onEnable() {
	Engine::physicsWorld->registerRigidbody(this);
}

void Rigidbody::onDisable() {
	Engine::physicsWorld->unregisterRigidbody(this);
}

void Rigidbody::setMass(float mass) {
	Engine::physicsWorld->unregisterRigidbody(this);
	this->mass = mass;
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.0f) {
		collider->getBulletShape()->calculateLocalInertia(mass, localInertia);
	}
	body->setMassProps(mass, localInertia);
	Engine::physicsWorld->registerRigidbody(this);
	if (mass != 0.0f) {
		body->activate();
	}
}

void Rigidbody::setKinematic(bool kinematic) {
	if (kinematic) {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	} else {
		body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(WANTS_DEACTIVATION);
	}
}

void Rigidbody::setCollider(Collider* collider) {
	Engine::physicsWorld->unregisterRigidbody(this);
	this->collider = collider;
	body->setCollisionShape(collider->getBulletShape());
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.0f) {
		this->collider->getBulletShape()->calculateLocalInertia(mass, localInertia);
	}
	body->setMassProps(mass, localInertia);
	Engine::physicsWorld->registerRigidbody(this);
}
