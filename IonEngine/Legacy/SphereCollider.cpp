#include "SphereCollider.h"

#include "bullet/btBulletDynamicsCommon.h"
using namespace IonEngine;



SphereCollider::SphereCollider(float radius) : Collider(new btSphereShape(radius)), radius(radius) {}

SphereCollider::~SphereCollider() {}
