#pragma once
#include "Collider.h"

class Mesh;

class MeshCollider : public Collider {
public:
	MeshCollider(Mesh* mesh);
	~MeshCollider();

	inline Mesh* getMesh() const { return mesh; }

protected:
	Mesh* mesh;
};

