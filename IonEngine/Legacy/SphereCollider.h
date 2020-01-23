#pragma once
#include "Collider.h"

namespace IonEngine {
	class SphereCollider : public Collider {
	public:
		SphereCollider(float radius);
		~SphereCollider();

		inline float getRadius() const { return radius; }

	private:
		float radius;
	};
}
