#include "BoxCollider.h"

#include "bullet/btBulletDynamicsCommon.h"



BoxCollider::BoxCollider(Vec3f halfExtents) : Collider(new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z))), halfExtents(halfExtents) {}

BoxCollider::~BoxCollider() {}
