#include "ConcaveMeshCollider.h"

#include <bullet/btBulletCollisionCommon.h>
#include "Mesh.h"
#include "BulletMesh.h"



ConcaveMeshCollider::ConcaveMeshCollider(Mesh* mesh, Vec3f scaling) : MeshCollider(mesh) {
	btMesh = new BulletMesh(mesh);
	shape = new btBvhTriangleMeshShape(btMesh, false);
	shape->setLocalScaling(btVector3(scaling.x, scaling.y, scaling.z));
}

ConcaveMeshCollider::~ConcaveMeshCollider() {}
