#include "MeshRenderer.h"

#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"
#include "Material.h"
#include "SpecializedShaderProgram.h"
using namespace IonEngine;



MeshRenderer::MeshRenderer(Entity* entity) : Renderer(entity) {
	
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::setMaterial(Material* mat) {
	Renderer::setMaterial(mat);
	modelMatrixLocation = mat->specializedProgram->getUniformLocation("modelMatrix");
}

void MeshRenderer::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void MeshRenderer::onUpdate() {
	recalculateWorldBounds();
}

void MeshRenderer::render() {
	if (mesh == nullptr) return;

	material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, entity->transform->getLocalToWorldMatrix());

	mesh->render();
}

void MeshRenderer::recalculateWorldBounds() {
	if (mesh == nullptr) return;
	// TODO optimize
	Vec3f corners[8];
	Boxf meshBounds = mesh->getBounds();
	Matrix4x4f transf = transform->getLocalToWorldMatrix();
	corners[0] = transf.multPoint(Vec3f(meshBounds.min.x, meshBounds.min.y, meshBounds.min.z));
	corners[1] = transf.multPoint(Vec3f(meshBounds.min.x, meshBounds.min.y, meshBounds.max.z));
	corners[2] = transf.multPoint(Vec3f(meshBounds.max.x, meshBounds.min.y, meshBounds.max.z));
	corners[3] = transf.multPoint(Vec3f(meshBounds.max.x, meshBounds.min.y, meshBounds.min.z));
	corners[4] = transf.multPoint(Vec3f(meshBounds.min.x, meshBounds.max.y, meshBounds.min.z));
	corners[5] = transf.multPoint(Vec3f(meshBounds.min.x, meshBounds.max.y, meshBounds.max.z));
	corners[6] = transf.multPoint(Vec3f(meshBounds.max.x, meshBounds.max.y, meshBounds.max.z));
	corners[7] = transf.multPoint(Vec3f(meshBounds.max.x, meshBounds.max.y, meshBounds.min.z));

	worldBounds = Boxf(Vec3f::positiveInfinity, Vec3f::negativeInfinity);
	for (int i = 0; i < 8; i++) {
		if (corners[i].x < worldBounds.min.x) {
			worldBounds.min.x = corners[i].x;
		}
		if (corners[i].y < worldBounds.min.y) {
			worldBounds.min.y = corners[i].y;
		}
		if (corners[i].z < worldBounds.min.z) {
			worldBounds.min.z = corners[i].z;
		}

		if (corners[i].x > worldBounds.max.x) {
			worldBounds.max.x = corners[i].x;
		}
		if (corners[i].y > worldBounds.max.y) {
			worldBounds.max.y = corners[i].y;
		}
		if (corners[i].z > worldBounds.max.z) {
			worldBounds.max.z = corners[i].z;
		}
	}
}
