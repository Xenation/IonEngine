#pragma once
#include "Scene.h"

class Mesh;
class Entity;
class ShaderProgram;
class Material;

class TestScene : public Scene {
public:
	TestScene();
	~TestScene();

	virtual void load() override;
	virtual void update() override;
	virtual void destroy() override;
private:
	Entity* camera;
	Entity* sun;
	Entity* rotatingEnt;
	Entity* ground;
	Entity* ball;
	Entity* refCube;
	Entity* mirCube;
	Entity* tests;
	Entity* intersecter;
	Entity* rayEntity;
	Mesh* cubeMesh;
	Mesh* sphereMesh;
};

