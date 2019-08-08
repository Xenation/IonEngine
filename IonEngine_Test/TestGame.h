#pragma once
#include <Game.h>

namespace IonEngine {
	class ShaderProgram;
	class Material;
}

class TestGame : public IonEngine::Game {
public:
	TestGame();
	~TestGame();

	virtual void initialize() override;
	virtual void preUpdate() override;
	virtual void postUpdate() override;
	virtual void cleanUp() override;

private:
	IonEngine::ShaderProgram* postTestShader;
	IonEngine::Material* postTestMaterial;
	IonEngine::ShaderProgram* testShader;
	IonEngine::Material* testMaterial;
	IonEngine::ShaderProgram* particleShader;
	IonEngine::Material* particleMaterial;
	IonEngine::Material* groundMaterial;
	IonEngine::Material* wallMaterial;
};

