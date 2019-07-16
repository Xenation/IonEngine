#include "CompoundCollider.h"

#include <bullet/btBulletCollisionCommon.h>



CompoundCollider::CompoundCollider() : Collider(new btCompoundShape()) {}

CompoundCollider::~CompoundCollider() {}


void CompoundCollider::addCollider(Collider* collider, Vec3f position, Quaternion rotation) {
	collider->_parent = this;
	((btCompoundShape*) shape)->addChildShape(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(position.x, position.y, position.z)), collider->getBulletShape());
}

void CompoundCollider::removeCollider(Collider* collider) {
	((btCompoundShape*) shape)->removeChildShape(collider->getBulletShape());
}
