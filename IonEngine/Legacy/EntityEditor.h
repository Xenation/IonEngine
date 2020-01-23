#pragma once
#include "HollowSet.h"
#include "EditorWindow.h"

namespace IonEngine {
	class Entity;
}

namespace IonEngine::Editor {
	class EntityEditor : public EditorWindow {
	public:
		Entity* entity;

		EntityEditor(Entity* entity);
		~EntityEditor();

		void drawGui() override;

		static EntityEditor* getActiveEditor(Entity* entity);

	private:
		static HollowSet<EntityEditor*> activeEditors;

		u32 id;
	};
}
