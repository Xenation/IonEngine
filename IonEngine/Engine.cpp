#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"
#include "Input.h"
#include "Time.h"
#include "EntityManager.h"
#include "Gui.h"
#include "Scene.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "VisualDebug.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;
EntityManager* Engine::entityManager = nullptr;
PhysicsWorld* Engine::physicsWorld = nullptr;
Gui* Engine::gui = nullptr;
Scene* Engine::scene = nullptr;
Game* Engine::game = nullptr;


void Engine::initialize(Game* gamePt,  Pipeline* pipelinePt, Gui* guiPt) {
	window = new Window();
	entityManager = new EntityManager();
	physicsWorld = new PhysicsWorld();
	if (pipelinePt == nullptr) {
		pipeline = new Pipeline(window->getWidth(), window->getHeight());
	} else {
		pipeline = pipelinePt;
	}
	if (guiPt == nullptr) {
		gui = new Gui(window);
	} else {
		gui = guiPt;
	}

	game = gamePt;
	game->initialize();
}

void Engine::loop() {
	while (!window->shouldClose()) {
		Time::computeFrameTimes();
		// PHYSICS UPDATE
		physicsWorld->simulate(Time::deltaTime);
		// INPUT
		Input::PollEvents();
		// UPDATE
		game->preUpdate();
		entityManager->updateEntities();
		if (scene != nullptr) {
			scene->update();
		}
		gui->update();
		game->postUpdate();
		// RENDER
		pipeline->render();
		gui->render();
		VisualDebug::render();
		// DISPLAY
		window->display();
	}
}

void Engine::destroy() {
	game->cleanUp();

	delete scene;
	delete entityManager;
	delete gui;
	delete pipeline;
	delete window;
}
