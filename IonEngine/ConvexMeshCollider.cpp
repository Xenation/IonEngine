#include "ConvexMeshCollider.h"

#include <bullet/btBulletCollisionCommon.h>
#include "Mesh.h"



ConvexMeshCollider::ConvexMeshCollider(Mesh* mesh, Vec3f scaling) : MeshCollider(mesh) {
	int stride;
	float* posPt = (float*) mesh->getAttributePointer(0, stride); // TODO very unsafe, assumes position is at attribute index 0
	shape = new btConvexHullShape(posPt, mesh->getVertexCount(), stride);
	shape->setLocalScaling(btVector3(scaling.x, scaling.y, scaling.z));
}

ConvexMeshCollider::~ConvexMeshCollider() {
	
}
