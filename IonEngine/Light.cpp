#include "Light.h"

#include "Transform.h"
#include "Engine.h"
#include "LightManager.h"
using namespace IonEngine;



Light::Light(Entity* entity, Type type) : Component(entity), type(type), color(Color::white) {
	Engine::lightManager->registerLight(this);
}

Light::~Light() {
	Engine::lightManager->unregisterLight(this);
}


Vec3f Light::getDirection() {
	return transform->forward();
}
