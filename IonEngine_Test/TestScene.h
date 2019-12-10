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
	IonEngine::Entity* mapRoot;
	IonEngine::Mesh* cubeMesh;
	IonEngine::Mesh* sphereMesh;
	IonEngine::Mesh* sponzaMesh;
};

