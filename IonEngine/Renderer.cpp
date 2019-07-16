#include "Renderer.h"

#include "Engine.h"
#include "Pipeline.h"
#include "ShaderProgram.h"
#include "Material.h"



Renderer::Renderer(Entity* entity) : Component(entity), material(Material::errorMaterial) {
	id = Engine::pipeline->registerRenderer(this);
}

Renderer::~Renderer() {
	Engine::pipeline->unregisterRenderer(id);
}

void Renderer::setMaterial(Material* mat) {
	if (material != nullptr) {
		material->renderers.remove(matID);
	}
	if (mat == nullptr) {
		material = Material::errorMaterial;
	} else {
		material = mat;
		matID = material->renderers.add(this);
	}
}
