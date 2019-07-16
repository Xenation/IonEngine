#pragma once

class Window;
class Pipeline;
class EntityManager;
class Scene;
class Gui;
class Game;
class PhysicsWorld;

class Engine {
public:
	static Window* window;
	static Pipeline* pipeline;
	static EntityManager* entityManager;
	static PhysicsWorld* physicsWorld;
	static Gui* gui;
	static Scene* scene;
	static Game* game;

	static void initialize(Game* game, Pipeline* pipeline = nullptr, Gui* gui = nullptr);
	static void loop();
	static void destroy();
private:
	Engine();
	Engine(const Engine&);
	~Engine();
};

