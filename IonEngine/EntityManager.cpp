#include "EntityManager.h"

#include <imgui.h>
#include "Debug.h"
#include "Entity.h"
#include "Time.h"

#define ENTITY_ARRAY_START_SIZE 64
#define ENTITY_ARRAY_INCREASE 64



EntityManager::EntityManager() 
	: entities(HollowSet<Entity*>(ENTITY_ARRAY_START_SIZE, ENTITY_ARRAY_INCREASE)) {
	for (int i = 0; i < 300; i++) {
		updateTimes[i] = 0;
	}
}

EntityManager::~EntityManager() {
	unsigned int deleted = 0;
	for (unsigned int i = 0; i < entities.capacity; i++) {
		if (deleted == entities.count) break;
		if (entities[i] == nullptr) continue;
		delete entities[i];
		deleted++;
	}
}

void EntityManager::registerEntity(Entity* entity) {
	entity->id = entities.add(entity);
}

void EntityManager::unregisterEntity(Entity* entity) {
	entities.remove(entity->id);
}

void EntityManager::updateEntities() {
	Time::beginTimeMesure();
	unsigned int updated = 0;
	for (unsigned int i = 0; i < entities.capacity; i++) {
		if (updated == entities.count) break;
		if (entities[i] == nullptr) continue;
		entities[i]->updateComponents();
		updated++;
	}
	addUpdateTime(Time::endTimeMesure());
}

void EntityManager::gui() {
	ImGui::Text("Update Times");
	ImGui::Indent();
	ImGui::PlotLines("", updateTimes, 300, 0, '\0', 0.0f, maxUpdateTime);
	ImGui::Text("Max Update Time: %.6fms", maxUpdateTime * 1000);
	ImGui::Unindent();
	ImGui::Text("Scene");
	unsigned int displayed = 0;
	for (unsigned int i = 0; i < entities.capacity; i++) {
		if (displayed == entities.count) break;
		if (entities[i] == nullptr || entities[i]->getParent() != nullptr) continue;
		gui(entities[i]);
		displayed++;
	}
}

void EntityManager::gui(Entity* entity) {
	if (ImGui::TreeNode(entity->name)) {
		HollowSet<Component*>& components = entity->getRawComponentsSet();
		if (components.count != 0 && ImGui::TreeNode("components")) {
			unsigned int compCount = 0;
			for (unsigned int i = 0; i < components.capacity && compCount < components.count; i = compCount = i + 1) {
				ImGui::Text(typeid(*components[i]).name());
			}
			ImGui::TreePop();
		}
		unsigned int childCount = entity->childCount();
		if (childCount != 0 && ImGui::TreeNode("childs")) {
			for (unsigned int i = 0; i < childCount; i++) {
				gui(entity->getChild(i));
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void EntityManager::addUpdateTime(float time) {
	maxUpdateTime = 0;
	for (int i = 0; i < 299; i++) {
		updateTimes[i] = updateTimes[i + 1];
		if (updateTimes[i] > maxUpdateTime) {
			maxUpdateTime = updateTimes[i];
		}
	}
	updateTimes[299] = time;
	if (time > maxUpdateTime) {
		maxUpdateTime = time;
	}
}
