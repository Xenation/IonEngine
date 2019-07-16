#include "TestGame.h"

#include <ShaderProgram.h>
#include <Material.h>



TestGame::TestGame() {}

TestGame::~TestGame() {}


void TestGame::initialize() {
	postTestShader = ShaderProgram::find("postprocess_test");
	postTestShader->load();
	postTestMaterial = new Material("Edge", postTestShader, "postprocess");
	postTestMaterial->setField(0, 0.0f);

	testShader = ShaderProgram::find("test");
	if (testShader != nullptr) {
		testShader->load();
	}
	testMaterial = new Material("Test", testShader, "opaque");
	testMaterial->setField(0, Color(1.0f, 0.5f, 0.0f));
	testMaterial->setField(1, Vec4f(2.0f, -3.0f, 1.0f, 0.0f));
	testMaterial->setField(2, 0.2f);

	particleShader = ShaderProgram::find("particle_basic");
	if (particleShader != nullptr) {
		particleShader->load();
	}
	particleMaterial = new Material("ParticleBasic", particleShader, "transparent");
	particleMaterial->setField(0, Color(1.0f, 0.5f, 0.0f));

	groundMaterial = new Material("Ground", testShader, "opaque");
	groundMaterial->setField(0, Color(0.5f, 0.5f, 0.0f));
	groundMaterial->setField(1, Vec4f(2.0f, -3.0f, 1.0f, 0.0f));
	groundMaterial->setField(2, 0.2f);

	wallMaterial = new Material("Wall", testShader, "opaque");
	wallMaterial->setField(0, Color(0.6f, 0.6f, 0.6f));
	wallMaterial->setField(1, Vec4f(2.0f, -3.0f, 1.0f, 0.0f));
	wallMaterial->setField(2, 0.2f);
}

void TestGame::preUpdate() {

}

void TestGame::postUpdate() {

}

void TestGame::cleanUp() {
	delete postTestMaterial;
	delete postTestShader;
	delete testMaterial;
	delete testShader;
	delete particleMaterial;
	delete particleShader;
}
