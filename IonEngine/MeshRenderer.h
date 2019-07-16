#pragma once
#include "Renderer.h"

class Mesh;
class Entity;

class MeshRenderer : public Renderer {
public:
	MeshRenderer(Entity* entity);
	~MeshRenderer();

	virtual void setMaterial(Material* mat) override;
	void setMesh(Mesh* mesh) { this->mesh = mesh; }
	inline Mesh* getMesh() { return mesh; }

	virtual void render() override;

private:
	Mesh* mesh = nullptr;
	unsigned int modelMatrixLocation = 0;
};

