#pragma once
#include <Scene.h>

namespace IonEngine {
	class Mesh;
	class Entity;
	class ShaderProgram;
	class Material;
}

class TestScene : public IonEngine::Scene {
public:
	TestScene();
	~TestScene();

	virtual void load() override;
	virtual void update() override;
	virtual void destroy() override;
private:
	IonEngine::Entity* camera;
	IonEngine::Entity* sun;
	IonEngine::Entity* rotatingEnt;
	IonEngine::Entity* ground;
	IonEngine::Entity* ball;
	IonEngine::Entity* refCube;
	IonEngine::Entity* mirCube;
	IonEngine::Entity* tests;
	IonEngine::Entity* intersecter;
	IonEngine::Entity* rayEntity;
	IonEngine::Mesh* cubeMesh;
	IonEngine::Mesh* sphereMesh;
};

