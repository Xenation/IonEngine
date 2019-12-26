#include "LightManager.h"

#include "Light.h"
using namespace IonEngine;



LightManager::LightManager() {}

LightManager::~LightManager() {}


void LightManager::registerLight(Light* light) {
	switch (light->getType()->id) {
	case LightType::DirectionalId:
		directionalLights.add(light);
		break;
	case LightType::PointId:
		pointLights.add(light);
		break;
	case LightType::SpotId:
		spotLights.add(light);
		break;
	}
}

void LightManager::unregisterLight(Light* light) {
	switch (light->getType()->id) {
	case LightType::DirectionalId:
		directionalLights.remove(light);
		break;
	case LightType::PointId:
		pointLights.remove(light);
		break;
	case LightType::SpotId:
		spotLights.remove(light);
		break;
	}
}

Light* LightManager::getMainDirectional() {
	if (directionalLights.count == 0) return nullptr;
	Light* mainDirectional = directionalLights[0];
	for (u32 i = 1; i < directionalLights.count; i++) {
		if (mainDirectional->intensity < directionalLights[i]->intensity) {
			mainDirectional = directionalLights[i];
		}
	}
	return mainDirectional;
}
