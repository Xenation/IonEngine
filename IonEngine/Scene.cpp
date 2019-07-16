#include "Scene.h"



Scene::Scene() {

}

Scene::~Scene() {
	if (isLoaded) destroy();
}

void Scene::load() {
	isLoaded = true;
}

void Scene::destroy() {
	isLoaded = false;
}
