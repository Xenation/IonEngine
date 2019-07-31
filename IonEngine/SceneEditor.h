#pragma once
#include "EditorWindow.h"

class Entity;
class EntityManager;

class SceneEditor : public EditorWindow {
public:
	SceneEditor(EntityManager* manager);
	~SceneEditor();

	void drawGui() override;
	void drawGui(Entity* entity);

private:
	EntityManager* manager;
};

