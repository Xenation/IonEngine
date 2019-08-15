#pragma once
#include "Component.h"
#include "XMath.h"

namespace IonEngine {
	class ShadowCaster;

	class Light : public Component {
	public:
		enum Type : unsigned int {
			Directional = 0,
			Point = 1,
			Spot = 2
		};

		union { // Use the alpha for intensity to save space
			Color color;
			struct { float colr, colg, colb, intensity; };
		};
		float range;
		float angle;
		float innerAngle;

		Light(Entity* entity, Type type);
		~Light();

		void onEnable() override;
		void onDisable() override;

		Type getType() const { return type; }
		void setType(Type type);
		Vec3f getDirection() const;
		Vec3f getPosition() const;
		bool isCastingShadow() const { return shadowCaster != nullptr; }
		void setCastShadow(bool castShadow);
		ShadowCaster* getShadowCaster() { return shadowCaster; }

	private:
		Type type = Type::Point;
		ShadowCaster* shadowCaster = nullptr;
	};
}
