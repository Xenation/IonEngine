#pragma once
#include "HollowSet.h"
#include "EditorWindow.h"

class Entity;

class EntityEditor : public EditorWindow {
public:
	Entity* entity;

	EntityEditor(Entity* entity);
	~EntityEditor();

	void drawGui() override;

	static EntityEditor* getActiveEditor(Entity* entity);

private:
	static HollowSet<EntityEditor*> activeEditors;

	unsigned int id;
};

