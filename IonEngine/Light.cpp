#include "Light.h"

#include "Transform.h"
#include "Engine.h"
#include "LightManager.h"
using namespace IonEngine;



Light::Light(Entity* entity, Type type) : Component(entity), type(type), color(Color::white) {
	
}

Light::~Light() {
	
}


void Light::onEnable() {
	Engine::lightManager->registerLight(this);
}

void Light::onDisable() {
	Engine::lightManager->unregisterLight(this);
}

void Light::setType(Light::Type type) {
	Engine::lightManager->unregisterLight(this);
	this->type = type;
	Engine::lightManager->registerLight(this);
}

Vec3f Light::getDirection() {
	return transform->forward();
}

Vec3f Light::getPosition() {
	return transform->getPosition();
}
