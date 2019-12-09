#include "Skybox.h"

#include "Mesh.h"
#include "MeshImporter.h"
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "Material.h"
using namespace IonEngine;



Mesh* Skybox::Skybox::skyMesh = nullptr;


Skybox::Skybox() {
	if (skyMesh == nullptr) {
		skyMesh = MeshImporter::import("icosphere_high.obj");
		skyMesh->reverseWindingOrder();
		skyMesh->uploadToGL();
	}

	ShaderProgram* procSkyShader = ShaderProgram::find("procedural_sky");
	unsigned int count;
	SpecializedShaderProgram* procSkySpecShader = procSkyShader->getAllSpecializedPrograms(count)[0];
	skyMaterial = new Material("Skybox", procSkySpecShader);
}

Skybox::~Skybox() {
}


void Skybox::render() {
	skyMaterial->use();
	skyMesh->render();
}
