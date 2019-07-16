#include "SphereCollider.h"

#include "bullet/btBulletDynamicsCommon.h"



SphereCollider::SphereCollider(float radius) : Collider(new btSphereShape(radius)) {}

SphereCollider::~SphereCollider() {}
