#include "TestGame.h"

#include <Engine.h>
#include <Window.h>
#include <ShaderProgram.h>
#include <Material.h>
using namespace IonEngine;



TestGame::TestGame() {}

TestGame::~TestGame() {}


void TestGame::initialize() {
	//Engine::window->disableVSync();

	//postTestShader = ShaderProgram::find("postprocess_test");
	//postTestShader->load();
	//postTestMaterial = new Material("Edge", postTestShader, "postprocess");
	//postTestMaterial->setField(0, 0.0f);

	testShader = ShaderProgram::find("test");
	if (testShader != nullptr) {
		testShader->load();
	}
	testMaterial = new Material("Test", testShader, "opaque");
	testMaterial->setField(0, Color(1.0f, 0.5f, 0.0f));
	testMaterial->setField(1, 0.1f);
	testMaterial->setField(2, 0.0f);
	testMaterial->setField(3, 0.3f);
	testMaterial->setField(4, 0.0f);

	particleShader = ShaderProgram::find("particle_basic");
	if (particleShader != nullptr) {
		particleShader->load();
	}
	particleMaterial = new Material("ParticleBasic", particleShader, "transparent");
	particleMaterial->setField(0, Color(1.0f, 0.5f, 0.0f));

	groundMaterial = new Material("Ground", testShader, "opaque");
	groundMaterial->setField(0, Color(0.5f, 0.5f, 0.0f));
	groundMaterial->setField(1, 0.1f);
	groundMaterial->setField(2, 0.0f);
	groundMaterial->setField(3, 0.3f);
	groundMaterial->setField(4, 0.0f);

	wallMaterial = new Material("Wall", testShader, "opaque");
	wallMaterial->setField(0, Color(0.6f, 0.6f, 0.6f));
	wallMaterial->setField(1, 0.1f);
	wallMaterial->setField(2, 0.0f);
	wallMaterial->setField(3, 0.3f);
	wallMaterial->setField(4, 0.0f);

	sponzaMaterial = new Material("Sponza", testShader, "opaque");
	sponzaMaterial->setField(0, Color(0.6f, 0.6f, 0.6f));
	sponzaMaterial->setField(1, 0.1f);
	sponzaMaterial->setField(2, 0.0f);
	sponzaMaterial->setField(3, 0.7f);
	sponzaMaterial->setField(4, 0.0f);
}

void TestGame::preUpdate() {

}

void TestGame::postUpdate() {

}

void TestGame::cleanUp() {
	//delete postTestMaterial;
	//delete postTestShader;
	delete testMaterial;
	delete wallMaterial;
	delete groundMaterial;
	delete sponzaMaterial;
	delete testShader;
	delete particleMaterial;
	delete particleShader;
}
