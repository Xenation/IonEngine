#pragma once
#include "Component.h"

class Material;

class Renderer : public Component {
public:
	Renderer(Entity* entity);
	~Renderer();

	virtual void render() = 0;

	virtual void setMaterial(Material* mat);
	Material* getMaterial() { return material; }

protected:
	Material* material = nullptr;

private:
	unsigned int id;
	unsigned int matID;
};

