#pragma once
#include "Renderer.h"

namespace IonEngine {
	class Mesh;
	class Entity;

	class MeshRenderer : public Renderer {
	public:
		MeshRenderer(Entity* entity);
		~MeshRenderer();

		virtual void setMaterial(Material* mat) override;
		void setMesh(Mesh* mesh);
		inline Mesh* getMesh() { return mesh; }

		virtual void onUpdate() override;
		virtual void render() override;

	private:
		Mesh* mesh = nullptr;
		unsigned int modelMatrixLocation = 0;

		void recalculateWorldBounds();
	};
}
