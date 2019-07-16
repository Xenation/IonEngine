#include "BulletMesh.h"

#include "Mesh.h"



BulletMesh::BulletMesh(Mesh* mesh) : mesh(mesh) {}

BulletMesh::~BulletMesh() {}


void BulletMesh::getLockedVertexIndexBase(unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart) {
	*vertexbase = (unsigned char*) mesh->getAttributePointer(0, stride); // TODO very unsafe, assumes position is at attribute index 0
	numverts = mesh->getVertexCount();
	type = PHY_ScalarType::PHY_FLOAT;
	*indexbase = (unsigned char*) mesh->getIndicesPointer();
	indexstride = 12;
	numfaces = mesh->getIndexCount() / 3;
	indicestype = PHY_ScalarType::PHY_INTEGER;
}

void BulletMesh::getLockedReadOnlyVertexIndexBase(const unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, const unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart) const {
	*vertexbase = (unsigned char*) mesh->getAttributePointer(0, stride); // TODO very unsafe, assumes position is at attribute index 0
	numverts = mesh->getVertexCount();
	type = PHY_ScalarType::PHY_FLOAT;
	*indexbase = (unsigned char*) mesh->getIndicesPointer();
	indexstride = 12;
	numfaces = mesh->getIndexCount() / 3;
	indicestype = PHY_ScalarType::PHY_INTEGER;
}

void BulletMesh::unLockVertexBase(int subpart) {

}

void BulletMesh::unLockReadOnlyVertexBase(int subpart) const {

}

int BulletMesh::getNumSubParts() const {
	return 1;
}

void BulletMesh::preallocateVertices(int numvertices) {

}

void BulletMesh::preallocateIndices(int numindices) {

}
