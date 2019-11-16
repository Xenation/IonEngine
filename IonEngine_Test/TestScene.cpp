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
	camera->transform->setRotation(Rotor3f::euler({ M_PI_4, -M_PI_4, 0 }));
	//Light* cameraLight = new Light(camera, LightType::Spot);
	////cameraLight->setCastShadow(true);
	//cameraLight->range = 50.0f;
	//cameraLight->angle = quarter_pi;
	//cameraLight->innerAngle = quarter_pi * 0.5f;
	//camera->addComponent(cameraLight);

	sun = new Entity("Sun");
	sun->transform->setPosition({-10, 10, -10});
	Light* sunLight = new Light(sun, LightType::Directional);
	sunLight->setCastShadow(true);
	sun->addComponent(sunLight);
	sun->transform->setRotation(Rotor3f::euler({M_PI_4, M_PI_4 * 0.5f, 0}));

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

	//Entity* particlesEnt = new Entity("Particles");
	//particlesEnt->setParent(noTransfChild);
	//particlesEnt->transform->setPosition(Vec3f(0, 2, 0));
	//MeshRenderer* particlesRend = particlesEnt->addComponent<MeshRenderer>();
	//particlesRend->setMaterial(testMaterial);
	//particlesRend->setMesh(cubeMesh);
	//ParticleSystem* particleSystem = particlesEnt->addComponent<ParticleSystem>();
	//particleSystem->emitVelocity = Vec3f::up * 5;
	//particleSystem->emitRate = 30;
	//particleSystem->minLifetime = 3;
	//particleSystem->maxLifetime = 5;
	//particleSystem->setMaterial(Material::find("ParticleBasic"));
	//particleSystem->startEmit();
	Entity* childEnt = new Entity("ChildCube");
	childEnt->setParent(noTransfChild);
	childEnt->transform->setPosition(Vec3f(0, 2, 0));
	MeshRenderer* childEntRend = childEnt->addComponent<MeshRenderer>();
	childEntRend->setMaterial(testMaterial);
	childEntRend->setMesh(cubeMesh);

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

	Entity* sphere = new Entity("Sphere");
	sphere->setParent(ground);
	sphere->transform->setWorldPosition({0, 0, 0});
	sphere->transform->setWorldScale({1, 1, 1});
	MeshRenderer* sphereRenderer = sphere->addComponent<MeshRenderer>();
	sphereRenderer->setMaterial(wallMaterial);
	sphereMesh = MeshImporter::Import("icosphere.obj");
	sphereMesh->uploadToGL();
	sphereRenderer->setMesh(sphereMesh);

	ball = new Entity("Cube");
	ball->transform->setPosition(Vec3f(0, 20, 0));
	MeshRenderer* ballRenderer = ball->addComponent<MeshRenderer>();
	ballRenderer->setMaterial(testMaterial);
	ballRenderer->setMesh(cubeMesh);
	Rigidbody* ballRb = ball->addComponent<Rigidbody>();
	ballRb->setCollider(ballCollider);
	ballRb->setMass(1.0f);
	ballRb->disable();

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
	


	refCube = new Entity("RefCube");
	refCube->transform->setPosition(Vec3f(5, 5, 0));
	MeshRenderer* refCubeRenderer = refCube->addComponent<MeshRenderer>();
	refCubeRenderer->setMaterial(testMaterial);
	refCubeRenderer->setMesh(cubeMesh);

	mirCube = new Entity("MirCube");
	mirCube->transform->setPosition(Vec3f(10, 5, 0));
	MeshRenderer* mirCubeRenderer = mirCube->addComponent<MeshRenderer>();
	mirCubeRenderer->setMaterial(testMaterial);
	mirCubeRenderer->setMesh(cubeMesh);

	tests = new Entity("Tests");

	intersecter = new Entity("Intersecter");
	intersecter->setParent(tests);
	intersecter->transform->setPosition(Vec3f(3, 0, 0));

	rayEntity = new Entity("Ray");
	rayEntity->setParent(tests);
	rayEntity->transform->setPosition(Vec3f(5, 0, 0));

}

//Frustum3f frustum = Frustum3f(0.1f, 20, 110, 1);
//Vec3f refCenter = Vec3f::zero;
void TestScene::update() {
	VisualDebug::drawLine(Vec3f::zero, Vec3f::forward, Color::blue);
	VisualDebug::drawLine(Vec3f::zero, Vec3f::right, Color::red);
	VisualDebug::drawLine(Vec3f::zero, Vec3f::up, Color::green);

	//Camera* cam = camera->getComponent<Camera>();
	//if (Input::getKeyDown(KeyCode::SPACE)) {
	//	frustum = Frustum3f(cam->getNearPlane(), 20, cam->getFov(), cam->getAspect());
	//	frustum.updateProjectionMatrix();
	//	frustum.updatePlanes(cam->getViewMatrix());
	//	refCenter = cam->transform->getPosition();
	//}
	//Debug::log("Frustum", "near" + (frustum.projectionMatrix * Vec4f(0, 0, -cam->getNearPlane(), 1)).toString());
	//Debug::log("Frustum", "far" + (frustum.projectionMatrix * Vec4f(0, 0, -20, 1)).toString());
	//VisualDebug::drawWirePlaneDisk(frustum.leftPlane, refCenter, 2, 6, Color::blue);
	//VisualDebug::drawWirePlaneDisk(frustum.rightPlane, refCenter, 2, 6, Color::cyan);
	//VisualDebug::drawWirePlaneDisk(frustum.topPlane, refCenter, 2, 6, Color::yellow);
	//VisualDebug::drawWirePlaneDisk(frustum.bottomPlane, refCenter, 2, 6, Color::orange);
	//VisualDebug::drawWirePlaneDisk(frustum.nearPlane, refCenter, 2, 6, Color::purple);
	//VisualDebug::drawWirePlaneDisk(frustum.farPlane, refCenter, 2, 6, Color::red);

	//Vec3f hit;
	//Boxf box = Boxf::fromCenterExtents(intersecter->transform->getPosition(), Vec3f::one * 0.5f);
	//VisualDebug::drawCube(box.getCenter(), box.getSize(), Color::orange);
	//Ray3f ray(rayEntity->transform->getPosition(), rayEntity->transform->forward());
	//if (box.intersect(ray, hit)) {
	//	VisualDebug::drawRay(ray, 10.0f, Color::red);
	//} else {
	//	VisualDebug::drawRay(ray, 10.0f, Color::blue);
	//}
	//VisualDebug::drawCube(hit, Vec3f::one * 0.05f, Color::magenta);

	//sun->getComponent<Light>()->intensity = sinf(Time::time) * 0.5f + 0.5f;
	//sun->getComponent<Light>()->color.r = sinf(Time::time) * .5f + .5f;
	//sun->getComponent<Light>()->color.g = sinf(Time::time + M_PI * 0.333f) * .5f + .5f;
	//sun->getComponent<Light>()->color.b = sinf(Time::time + M_PI * 0.666f) * .5f + .5f;
	//sun->transform->setWorldRotation(Rotor3f::euler(M_PI_4, Time::time, 0));

	Rigidbody* ballRb = ball->getComponent<Rigidbody>();
	if (Time::time > 5.0f && !ballRb->isEnabled()) {
		ballRb->enable();
	}

	Vec3f euler = Vec3f(Time::time, Time::time * 0.4f, Time::time * 0.25f);
	Rotor3f rotor = Rotor3f::euler(euler);
	refCube->transform->setRotation(rotor);
	//Matrix4x4f transf = Matrix4x4f::transformation(Vec3f::zero, Vec3f::one, rotor);
	Rotor3f extractedRotor = refCube->transform->getLocalToParentMatrix().rotor();
	mirCube->transform->setRotation(extractedRotor);
	//if (rotor != extractedRotor) {
	//	Debug::log("ROTOR", "different extracted rotor! original:" + rotor.toString() + "  extracted:" + extractedRotor.toString());
	//}

	//Random rng(666);
	//for (int i = 0; i < 666; i++) {
	//	Vec3f pos = toVec3f(rng.inCircle());
	//	VisualDebug::drawCube(pos, Vec3f::one * 0.01f, rng.color());
	//}

	//VisualDebug::drawFrustum(camera->getComponent<Camera>()->getViewMatrix(), camera->getComponent<Camera>()->getProjectionMatrix(), camera->getComponent<Camera>()->getNearPlane(), camera->getComponent<Camera>()->getFarPlane() - 500, Color::red);
}

void TestScene::destroy() {
	Scene::destroy();
	delete camera;
	delete sun;
	delete rotatingEnt;
	delete ground;
	delete ball;
	delete refCube;
	delete mirCube;
	delete cubeMesh;
	delete sphereMesh;
}
