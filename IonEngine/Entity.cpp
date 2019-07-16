#include "Entity.h"

#include "Debug.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Transform.h"

#define COMPONENTS_ARRAY_START_SIZE 4
#define COMPONENTS_ARRAY_INCREASE 1
#define CHILDREN_ARRAY_START_SIZE 2
#define CHILDREN_ARRAY_INCREASE 2



Entity::Entity(const char* name, bool hasTransform)
	: name(name), components(HollowSet<Component*>(COMPONENTS_ARRAY_START_SIZE, COMPONENTS_ARRAY_INCREASE)), children(CHILDREN_ARRAY_START_SIZE, CHILDREN_ARRAY_INCREASE, true) {
	if (hasTransform) {
		transform = addComponent(new Transform(this));
	}
	Engine::entityManager->registerEntity(this);
}

Entity::~Entity() {
	isDying = true;
	Engine::entityManager->unregisterEntity(this);
	unsigned int deletedCount = 0;
	for (unsigned int i = 0; i < components.capacity; i++) {
		if (deletedCount == components.count) break;
		if (components[i] == nullptr) continue;
		if (components[i]->isEnabled()) {
			components[i]->onDisable();
		}
		components[i]->onDestroy();
		delete components[i];
		deletedCount++;
	}
	if (parent != nullptr) {
		parent->removeChild(childIndex);
	}
	for (unsigned int i = 0; i < children.count; i++) {
		delete children[i];
	}
}

void Entity::updateComponents() {
	unsigned int updated = 0;
	for (unsigned int i = 0; i < components.capacity; i++) {
		if (updated == components.count) break;
		if (components[i] == nullptr) continue;
		components[i]->onUpdate();
		updated++;
	}
}

void Entity::updateTransformPointers() {
	unsigned int updated = 0;
	for (unsigned int i = 0; i < components.capacity; i++) {
		if (updated == components.count) break;
		if (components[i] == nullptr) continue;
		components[i]->transform = transform;
		updated++;
	}
}

void Entity::setParent(Entity* parent) {
	if (this->parent == parent) return;
	if (this->parent != nullptr) {
		this->parent->removeChild(childIndex);
	}
	this->parent = parent;
	if (this->parent != nullptr) {
		this->parent->addChild(this);
	}
	parentChangeNotifyChildren();
}

Entity* Entity::getChild(unsigned int index) {
	if (index < 0 || index >= children.count) return nullptr;
	return children[index];
}

void Entity::addChild(Entity* child) {
	child->childIndex = children.add(child);
}

void Entity::removeChild(unsigned int index) {
	children.remove(index);
	for (unsigned int i = index; i < children.count; i++) {
		children[i]->childIndex--;
	}
}

void Entity::parentChangeNotifyChildren() {
	if (transform != nullptr) {
		transform->parentChanged();
	}
	for (unsigned int i = 0; i < children.count; i++) {
		children[i]->parentChangeNotifyChildren();
	}
}

void Entity::ltwChangeNotifyChildren() {
	for (unsigned int i = 0; i < children.count; i++) {
		children[i]->parentChangeNotifyChildren();
	}
}
