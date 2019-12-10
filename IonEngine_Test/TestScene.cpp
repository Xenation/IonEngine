#include "TestScene.h"

#include <Material.h>
#include <ParticleSystem.h>
#include <Rigidbody.h>
#include <Mesh.h>
#include <Entity.h>
#include <MeshRenderer.h>
#include <Camera.h>
#include <Transform.h>
#include <ShaderProgram.h>
#include <Time.h>
#include <BoxCollider.h>
#include <ConvexMeshCollider.h>
#include <ConcaveMeshCollider.h>
#include <SphereCollider.h>
#include <Random.h>
#include <VisualDebug.h>
#include <Light.h>
#include <MeshImporter.h>
#include <Input.h>
#include "NoclipController.h"
#include "TestRotator.h"
using namespace IonEngine;



TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::load() {
	Scene::load();
	camera = new Entity("Camera");
	camera->addComponent<Camera>();
	camera->addComponent<NoclipController>()->lookSensivity = 0.5f;
	camera->transform->setPosition({5, 5, -5});
	camera->transform->setRotation(Rotor3f::euler({Math::quarter_pi, -Math::quarter_pi, 0}));

	sun = new Entity("Sun");
	sun->transform->setPosition({-10, 10, -10});
	Light* sunLight = new Light(sun, LightType::Directional);
	sunLight->setCastShadow(true);
	sun->addComponent(sunLight);
	sun->transform->setRotation(Rotor3f::euler({60.0f * Math::deg2rad, 22.5f * Math::deg2rad, 0}));

	cubeMesh = new Mesh("Cube", 24, 36);
	cubeMesh->setAttributesDefinition(2, new int[2]{3, 3});
	cubeMesh->setAttribute(0, new float[72]{
		// Front
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		// Top
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		// Bottom
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		// Left
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		// Right
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		// Back
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
	});
	cubeMesh->setAttribute(1, new float[72]{
		// Front
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	});
	cubeMesh->setIndices(new unsigned int[36]{
		0, 2, 1,
		0, 3, 2,
		4, 6, 5,
		4, 7, 6,
		8, 10, 9,
		8, 11, 10,
		12, 14, 13,
		12, 15, 14,
		16, 18, 17,
		16, 19, 18,
		20, 22, 21,
		20, 23, 22
	});
	cubeMesh->uploadToGL();

	Material* testMaterial = Material::find("Test");
	Material* groundMaterial = Material::find("Ground");
	Material* wallMaterial = Material::find("Wall");
	Material* sponzaMaterial = Material::find("Sponza");

	rotatingEnt = new Entity("Rotating");
	rotatingEnt->addComponent<TestRotator>();
	rotatingEnt->transform->setPosition(Vec3f(-5, 2, 0));
	MeshRenderer* rotatingRend = rotatingEnt->addComponent<MeshRenderer>();
	rotatingRend->setMaterial(testMaterial);
	rotatingRend->setMesh(cubeMesh);

	Entity* noTransfChild = new Entity("EntityNoTransform", false);
	noTransfChild->setParent(rotatingEnt);

	Entity* childEnt = new Entity("ChildCube");
	childEnt->setParent(noTransfChild);
	childEnt->transform->setPosition(Vec3f(0, 2, 0));
	MeshRenderer* childEntRend = childEnt->addComponent<MeshRenderer>();
	childEntRend->setMaterial(testMaterial);
	childEntRend->setMesh(cubeMesh);

	Entity* sphere = new Entity("Sphere");
	sphere->setParent(mapRoot);
	sphere->transform->setWorldPosition({0, 0, 0});
	sphere->transform->setWorldScale({1, 1, 1});
	MeshRenderer* sphereRenderer = sphere->addComponent<MeshRenderer>();
	sphereRenderer->setMaterial(wallMaterial);
	sphereMesh = MeshImporter::import("icosphere_high.obj");
	sphereMesh->uploadToGL();
	sphereRenderer->setMesh(sphereMesh);

	// Map
	mapRoot = new Entity("Map");

	Entity* sponza = MeshImporter::importAll("sponza.obj");
	sponza->setParent(mapRoot);
	sponza->transform->setWorldPosition({0.0f, -2.0f, 0.0f});
	sponza->transform->setWorldScale({0.015f, 0.015f, 0.015f});

	Entity* dynLightEntity = new Entity("Light");
	dynLightEntity->transform->setPosition({6, 10, 0});
	dynLightEntity->transform->setRotation(Rotor3f::euler(half_pi, 0, 0));
	Light* pointLight = new Light(dynLightEntity, LightType::Point);
	pointLight->range = 20.0f;
	pointLight->angle = quarter_pi;
	pointLight->innerAngle = quarter_pi * 0.5f;
	//pointLight->setCastShadow(true);
	dynLightEntity->addComponent(pointLight);

	Entity* dynLightEntity2 = new Entity("Light2");
	dynLightEntity2->transform->setPosition({-10, 10, 0});
	dynLightEntity2->transform->setRotation(Rotor3f::euler(half_pi, 0, 0));
	Light* dynLight2 = new Light(dynLightEntity2, LightType::Spot);
	dynLight2->range = 20.0f;
	dynLight2->angle = quarter_pi;
	dynLight2->innerAngle = quarter_pi * 0.5f;
	dynLightEntity2->addComponent(dynLight2);

}

void TestScene::update() {
	VisualDebug::drawLine(Vec3f::zero, Vec3f::forward, Color::blue);
	VisualDebug::drawLine(Vec3f::zero, Vec3f::right, Color::red);
	VisualDebug::drawLine(Vec3f::zero, Vec3f::up, Color::green);
}

void TestScene::destroy() {
	Scene::destroy();
	delete camera;
	delete sun;
	delete rotatingEnt;
	delete mapRoot;
	delete cubeMesh;
	delete sphereMesh;
	delete sponzaMesh;
}
