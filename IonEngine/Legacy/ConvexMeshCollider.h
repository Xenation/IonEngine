#pragma once
#include "MeshCollider.h"
#include "XMath.h"

namespace IonEngine {
	class ConvexMeshCollider : public MeshCollider {
	public:
		ConvexMeshCollider(Mesh* mesh, Vec3f scaling = Vec3f::one);
		~ConvexMeshCollider();
	};
}
