#include "Component.h"

#include "Entity.h"
#include "Transform.h"



Component::Component(Entity* entity) : entity(entity), transform(entity->transform) {

}

Component::Component(Entity* entity, Transform* transform) : entity(entity), transform(transform) {

}

Component::~Component() {
	if (entity->isDying) return;
	entity->removeComponent(this);
}

void Component::enable() {
	this->enabled = true;
	onEnable();
}

void Component::disable() {
	this->enabled = false;
	onDisable();
}
