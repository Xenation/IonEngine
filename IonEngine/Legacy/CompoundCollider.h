#pragma once
#include "Collider.h"
#include "XMath.h"

namespace IonEngine {
	class CompoundCollider : public Collider {
	public:
		CompoundCollider();
		~CompoundCollider();

		void addCollider(Collider* collider, Vec3f position, Quaternion rotation);
		void removeCollider(Collider* collider);
	};
}
