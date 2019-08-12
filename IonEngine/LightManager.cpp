#include "LightManager.h"

#include "Light.h"
using namespace IonEngine;



LightManager::LightManager() {}

LightManager::~LightManager() {}


void LightManager::registerLight(Light* light) {
	switch (light->getType()) {
	case Light::Directional:
		directionalLights.add(light);
		break;
	case Light::Point:
		pointLights.add(light);
		break;
	case Light::Spot:
		spotLights.add(light);
		break;
	}
}

void LightManager::unregisterLight(Light* light) {
	switch (light->getType()) {
	case Light::Directional:
		directionalLights.remove(light);
		break;
	case Light::Point:
		pointLights.remove(light);
		break;
	case Light::Spot:
		spotLights.remove(light);
		break;
	}
}

Light* LightManager::getMainDirectional() {
	if (directionalLights.count == 0) return nullptr;
	Light* mainDirectional = directionalLights[0];
	for (unsigned int i = 1; i < directionalLights.count; i++) {
		if (mainDirectional->intensity < directionalLights[i]->intensity) {
			mainDirectional = directionalLights[i];
		}
	}
	return mainDirectional;
}
