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
#include "NoclipController.h"
#include "TestRotator.h"



TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::load() {
	Scene::load();
	camera = new Entity("Camera");
	camera->addComponent<Camera>();
	camera->addComponent<NoclipController>()->lookSensivity = 1.0f;
	camera->transform->setPosition({5, 5, -5});
	camera->transform->setRotation(Quaternion::euler({ M_PI_4, -M_PI_4, 0 }));

	cubeMesh = new Mesh("Cube", 8, 36);
	cubeMesh->setAttributesDefinition(1, new int[1]{3});
	cubeMesh->setAttribute(0, new float[24]{
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
	});
	cubeMesh->setIndices(new unsigned int[36]{
		0, 2, 1,
		0, 3, 2,
		4, 3, 0,
		4, 7, 3,
		5, 0, 1,
		5, 4, 0,
		6, 1, 2,
		6, 5, 1,
		7, 2, 3,
		7, 6, 2,
		5, 7, 4,
		5, 6, 7
	});
	cubeMesh->uploadToGL();

	Material* testMaterial = Material::find("Test");
	Material* groundMaterial = Material::find("Ground");
	Material* wallMaterial = Material::find("Wall");

	rotatingEnt = new Entity("Rotating");
	rotatingEnt->addComponent<TestRotator>();
	rotatingEnt->transform->setPosition(Vec3f(-5, 2, 0));
	MeshRenderer* rotatingRend = rotatingEnt->addComponent<MeshRenderer>();
	rotatingRend->setMaterial(testMaterial);
	rotatingRend->setMesh(cubeMesh);

	Entity* noTransfChild = new Entity("EntityNoTransform", false);
	noTransfChild->setParent(rotatingEnt);

	Entity* particlesEnt = new Entity("Particles");
	particlesEnt->setParent(noTransfChild);
	particlesEnt->transform->setPosition(Vec3f(0, 2, 0));
	MeshRenderer* particlesRend = particlesEnt->addComponent<MeshRenderer>();
	particlesRend->setMaterial(testMaterial);
	particlesRend->setMesh(cubeMesh);
	ParticleSystem* particleSystem = particlesEnt->addComponent<ParticleSystem>();
	particleSystem->emitVelocity = Vec3f::up * 5;
	particleSystem->emitRate = 30;
	particleSystem->minLifetime = 3;
	particleSystem->maxLifetime = 5;
	particleSystem->setMaterial(Material::find("ParticleBasic"));
	particleSystem->startEmit();

	Collider* groundCollider = new ConcaveMeshCollider(cubeMesh, {100, 10, 100});
	Collider* groundWallCollider = new ConvexMeshCollider(cubeMesh, {2, 2, 10});
	Collider* ballCollider = new ConvexMeshCollider(cubeMesh);

	ground = new Entity("Ground");
	ground->transform->setPosition(Vec3f(0, -10, 0));
	ground->transform->setScale(Vec3f(100.0f, 10.0f, 100.0f));
	//ground->transform->setRotation(Quaternion::euler({0.0f, 0.0f, M_PI_4 / 4.0f}));
	MeshRenderer* groundRenderer = ground->addComponent<MeshRenderer>();
	groundRenderer->setMaterial(groundMaterial);
	groundRenderer->setMesh(cubeMesh);
	Rigidbody* groundRb = ground->addComponent<Rigidbody>();
	groundRb->setCollider(groundCollider);

	Entity* groundWall = new Entity("GroundWall");
	groundWall->setParent(ground);
	groundWall->transform->setWorldPosition({ -1, -4, 0 });
	groundWall->transform->setWorldScale({ 2, 2, 10 });
	MeshRenderer* groundWallRenderer = groundWall->addComponent<MeshRenderer>();
	groundWallRenderer->setMaterial(wallMaterial);
	groundWallRenderer->setMesh(cubeMesh);
	Rigidbody* groundWallRb = groundWall->addComponent<Rigidbody>();
	groundWallRb->setCollider(groundWallCollider);

	ball = new Entity("Cube");
	ball->transform->setPosition(Vec3f(0, 20, 0));
	MeshRenderer* ballRenderer = ball->addComponent<MeshRenderer>();
	ballRenderer->setMaterial(testMaterial);
	ballRenderer->setMesh(cubeMesh);
	Rigidbody* ballRb = ball->addComponent<Rigidbody>();
	ballRb->setCollider(ballCollider);
	ballRb->setMass(1.0f);
	ballRb->disable();
}

void TestScene::update() {
	Rigidbody* ballRb = ball->getComponent<Rigidbody>();
	if (Time::time > 15.0f && !ballRb->isEnabled()) {
		ballRb->enable();
	}
}

void TestScene::destroy() {
	Scene::destroy();
	delete camera;
	delete rotatingEnt;
	delete ground;
	delete ball;
	delete cubeMesh;
}
