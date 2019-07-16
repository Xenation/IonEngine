#include "Collider.h"

#include <bullet/btBulletCollisionCommon.h>
#include "CompoundCollider.h"



Collider*const Collider::empty = new Collider(new btEmptyShape());


Collider::Collider() {}

Collider::Collider(btCollisionShape* shape) : shape(shape) {}

Collider::~Collider() {
	if (_parent != nullptr) {
		_parent->removeCollider(this);
	}
}
