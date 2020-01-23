#include "EntityManager.h"

#include <imgui.h>
#include "Debug.h"
#include "Entity.h"
#include "Time.h"
#include "EntityEditor.h"
using namespace IonEngine;

#define ENTITY_ARRAY_START_SIZE 64
#define ENTITY_ARRAY_INCREASE 64



EntityManager::EntityManager() 
	: entities(HollowSet<Entity*>(ENTITY_ARRAY_START_SIZE, ENTITY_ARRAY_INCREASE)) {
	for (u32 i = 0; i < 300; i++) {
		updateTimes[i] = 0;
	}
}

EntityManager::~EntityManager() {
	u32 deleted = 0;
	for (u32 i = 0; i < entities.capacity; i++) {
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
	u32 updated = 0;
	for (u32 i = 0; i < entities.capacity; i++) {
		if (updated == entities.count) break;
		if (entities[i] == nullptr) continue;
		entities[i]->updateComponents();
		updated++;
	}
	addUpdateTime(Time::endTimeMesure());
}

void EntityManager::addUpdateTime(float time) {
	maxUpdateTime = 0;
	for (u32 i = 0; i < 299; i++) {
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
