#pragma once
#include "Collider.h"
#include "XMath.h"

namespace IonEngine {
	class BoxCollider : public Collider {
	public:
		BoxCollider(Vec3f halfExtents);
		~BoxCollider();

		inline Vec3f getHalfExtents() const { return halfExtents; }

	protected:
		Vec3f halfExtents;
	};
}
