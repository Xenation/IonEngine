#include "Light.h"

#include "Transform.h"
#include "Engine.h"
#include "LightManager.h"
#include "ShadowCaster.h"
#include "MeshImporter.h"
using namespace IonEngine;



const LightType*const LightType::Directional = new LightType(LightType::DirectionalId, nullptr);
const LightType*const LightType::Point = new LightType(LightType::PointId, nullptr);
const LightType*const LightType::Spot = new LightType(LightType::SpotId, nullptr);


Light::Light(Entity* entity, const LightType* type) : Component(entity), type(type), color(Color::white) {
	
}

Light::~Light() {
	if (shadowCaster != nullptr) {
		delete shadowCaster;
	}
}


void Light::onEnable() {
	Engine::lightManager->registerLight(this);
}

void Light::onDisable() {
	Engine::lightManager->unregisterLight(this);
}

void Light::setType(const LightType* type) {
	Engine::lightManager->unregisterLight(this);
	this->type = type;
	Engine::lightManager->registerLight(this);
}

Vec3f Light::getDirection() const {
	return transform->forward();
}

Vec3f Light::getPosition() const {
	return transform->getPosition();
}

void Light::setCastShadow(bool castShadow) {
	if (shadowCaster == nullptr && castShadow) {
		shadowCaster = new ShadowCaster(this);
	} else if (shadowCaster != nullptr && !castShadow) {
		delete shadowCaster;
		shadowCaster = nullptr;
	}
}
