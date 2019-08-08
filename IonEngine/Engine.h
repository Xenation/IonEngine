#pragma once

namespace IonEngine {
	namespace Editor {
		class Gui;
	}
	class Window;
	class Pipeline;
	class EntityManager;
	class Scene;
	class Game;
	class PhysicsWorld;
	class LightManager;

	class Engine {
	public:
		static Window* window;
		static Pipeline* pipeline;
		static EntityManager* entityManager;
		static PhysicsWorld* physicsWorld;
		static LightManager* lightManager; // TODO should be moved to a better place
		static Editor::Gui* gui;
		static Scene* scene;
		static Game* game;

		static void initialize(Game* game, Pipeline* pipeline = nullptr, Editor::Gui* gui = nullptr);
		static void loop();
		static void destroy();
	private:
		Engine();
		Engine(const Engine&);
		~Engine();
	};
}
