#include "Scene.h"

using namespace IonEngine;



Entity* Scene::createEntity() {
	return entities.allocate();
}
