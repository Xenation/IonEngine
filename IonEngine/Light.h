#pragma once
#include "Component.h"
#include "XMath.h"

namespace IonEngine {
	class ShadowCaster;
	class Mesh;

	struct LightType {
		static constexpr unsigned int DirectionalId = 0;
		static const LightType*const Directional;
		static constexpr unsigned int PointId = 1;
		static const LightType*const Point;
		static constexpr unsigned int SpotId = 2;
		static const LightType*const Spot;

		const unsigned int id = 0;
		Mesh*const cullingMesh = nullptr;

	public:
		LightType(unsigned int id, Mesh* mesh) : id(id), cullingMesh(mesh) {}

		static constexpr const LightType* fromId(unsigned int id) {
			switch (id) {
			case LightType::DirectionalId:
				return LightType::Directional;
			case LightType::PointId:
				return LightType::Point;
			case LightType::SpotId:
				return LightType::Spot;

			default:
				return LightType::Directional;
			}
		}
	};

	class Light : public Component {
	public:
		union { // Use the alpha for intensity to save space
			Color color;
			struct { float colr, colg, colb, intensity; };
		};
		float range;
		float angle;
		float innerAngle;

		Light(Entity* entity, const LightType* type);
		~Light();

		void onEnable() override;
		void onDisable() override;

		const LightType* getType() const { return type; }
		void setType(const LightType* type);
		Vec3f getDirection() const;
		Vec3f getPosition() const;
		bool isCastingShadow() const { return shadowCaster != nullptr; }
		void setCastShadow(bool castShadow);
		ShadowCaster* getShadowCaster() { return shadowCaster; }

	private:
		const LightType* type = LightType::Point;
		ShadowCaster* shadowCaster = nullptr;
	};
}
