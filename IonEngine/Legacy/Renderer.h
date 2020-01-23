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
		virtual void renderNoUniform() = 0; // TODO remove when using batched UBOs for per object variables

		virtual void onEnable() override;
		virtual void onDisable() override;

		virtual void setMaterial(Material* mat);
		Material* getMaterial() { return material; }
		Boxf getWorldBounds() { return worldBounds; }
		void setWorldBounds(Boxf bounds) { worldBounds = bounds; }

		u32 getID() { return id; }

	protected:
		Material* material = nullptr;
		Boxf worldBounds = Boxf(Vec3f::nan, Vec3f::nan);

	private:
		u32 id;
		u32 matID;
	};
}
