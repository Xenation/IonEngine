#pragma once
#include "Component.h"
#include "XMath.h"

namespace IonEngine {
	class Material;

	class Renderer : public Component {
	public:
		Renderer(Entity* entity);
		~Renderer();

		virtual void render() = 0;

		virtual void onEnable() override;
		virtual void onDisable() override;

		virtual void setMaterial(Material* mat);
		Material* getMaterial() { return material; }
		Boxf getWorldBounds() { return worldBounds; }
		void setWorldBounds(Boxf bounds) { worldBounds = bounds; }

		unsigned int getID() { return id; }

	protected:
		Material* material = nullptr;
		Boxf worldBounds = Boxf(Vec3f::zero, Vec3f::zero);

	private:
		unsigned int id;
		unsigned int matID;
	};
}
