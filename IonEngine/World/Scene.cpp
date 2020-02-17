#include "Scene.h"

using namespace IonEngine;



ComponentId Scene::componentCounter = 0;


Entity* Scene::createEntity() {
	return entities.allocate();
}

void Scene::destroyEntity(Entity* entity) {
	for (ComponentPool& pool : componentPools) {
		pool.freeComponent(entity);
	}
}
