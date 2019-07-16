#pragma once
#include <bullet/btBulletCollisionCommon.h>

class Mesh;

class BulletMesh : public btStridingMeshInterface {
public:
	BulletMesh(Mesh* mesh);
	~BulletMesh();

	virtual void getLockedVertexIndexBase(unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0) override;
	virtual void getLockedReadOnlyVertexIndexBase(const unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, const unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0) const override;
	virtual void unLockVertexBase(int subpart) override;
	virtual void unLockReadOnlyVertexBase(int subpart) const override;
	virtual int getNumSubParts() const override;
	virtual void preallocateVertices(int numvertices) override;
	virtual void preallocateIndices(int numindices) override;

private:
	Mesh* mesh;
};

