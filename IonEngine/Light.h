#pragma once
#include "Component.h"
#include "XMath.h"

namespace IonEngine {
	class Light : public Component {
	public:
		enum Type : unsigned int {
			Directional = 0,
			Point = 1,
			Spot = 2
		};

		const Type type = Type::Point;
		union { // Use the alpha for intensity to save space
			Color color;
			struct { float colr, colg, colb, intensity; };
		};
		bool castShadow = false;
		float range;
		float angle;
		float innerAngle;

		Light(Entity* entity, Type type);
		~Light();

		Vec3f getDirection();
	};
}
