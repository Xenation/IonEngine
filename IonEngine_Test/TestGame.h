#pragma once
#include <Game.h>

class ShaderProgram;
class Material;

class TestGame : public Game {
public:
	TestGame();
	~TestGame();

	virtual void initialize() override;
	virtual void preUpdate() override;
	virtual void postUpdate() override;
	virtual void cleanUp() override;

private:
	ShaderProgram* postTestShader;
	Material* postTestMaterial;
	ShaderProgram* testShader;
	Material* testMaterial;
	ShaderProgram* particleShader;
	Material* particleMaterial;
	Material* groundMaterial;
	Material* wallMaterial;
};

