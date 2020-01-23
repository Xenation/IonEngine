#pragma once
#include "MeshCollider.h"
#include "XMath.h"

namespace IonEngine {
	class BulletMesh;

	class ConcaveMeshCollider : public MeshCollider {
	public:
		ConcaveMeshCollider(Mesh* mesh, Vec3f scaling = Vec3f::one);
		~ConcaveMeshCollider();

	private:
		BulletMesh* btMesh;
	};
}
