#pragma once
#include "EditorWindow.h"

namespace IonEngine {
	class Entity;
	class EntityManager;
}

namespace IonEngine::Editor {
	class SceneEditor : public EditorWindow {
	public:
		SceneEditor(EntityManager* manager);
		~SceneEditor();

		void drawGui() override;
		void drawGui(Entity* entity);

	private:
		EntityManager* manager;
	};
}
