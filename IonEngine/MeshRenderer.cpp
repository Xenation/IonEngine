#include "MeshRenderer.h"

#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"
#include "Material.h"
#include "SpecializedShaderProgram.h"



MeshRenderer::MeshRenderer(Entity* entity) : Renderer(entity) {
	
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::setMaterial(Material* mat) {
	Renderer::setMaterial(mat);
	modelMatrixLocation = mat->specializedProgram->getUniformLocation("modelMatrix");
}

void MeshRenderer::render() {
	if (mesh == nullptr) return;

	material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, entity->transform->getLocalToWorldMatrix());

	mesh->render();
}
