#pragma once
#include <type_traits>
#include <typeinfo>
#include "Component.h"
#include "HollowSet.h"
#include "SimpleList.h"
#include "Transform.h"
class Entity {
public:
	typedef unsigned int Id;
	Id id;
	const char* name;
	Transform* transform = nullptr;
	bool isDying = false;

	Entity(const char* name = "Entity", bool hasTransform = true);
	Entity(const Entity&) = delete;
	~Entity();

	// Adds a component by creating a new
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent() {
		T* nComponent = new T(this);
		components.add(nComponent);

		if (typeid(T) == typeid(Transform)) {
			transform = (Transform*) nComponent;
			updateTransformPointers();
		}

		nComponent->onStart();
		if (nComponent->isEnabled()) {
			nComponent->onEnable();
		}
		return nComponent;
	}
	
	// Gets a component of the given type
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* getComponent() {
		for (unsigned int i = 0; i < components.capacity; i++) {
			if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) return (T*) components[i];
		}
		return nullptr;
	}

	// Gets all the components of the given type
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T** getComponents() {
		unsigned int count = 0;
		for (unsigned int i = 0; i < components.capacity; i++) {
			if (count == components.count) break;
			if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) count++;
		}
		T** comps = new Component*[count];
		getComponents(comps, count);
		return comps;
	}

	// Gets all the components of the given type using a preallocated array, returns the count of components found
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	unsigned int getComponents(T** arr, unsigned int arrSize) {
		unsigned int arrIndex = 0;
		for (unsigned int i = 0; i < components.capacity && arrIndex < arrSize; i++) {
			if (arrIndex == components.count) break;
			if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) {
				arr[arrIndex++] = components[i];
			}
		}
		return arrIndex;
	}

	// Removes a previously added component
	void removeComponent(Component* comp) {
		for (unsigned int i = 0; i < components.count; i++) {
			if (components[i] != nullptr && components[i] == comp) {
				if (components[i]->isEnabled()) {
					components[i]->onDisable();
				}
				components[i]->onDestroy();

				components[i] = nullptr;
				components.count--;
				break;
			}
		}
	}

	void updateComponents();

	void setParent(Entity* parent);
	inline Entity* getParent() const { return parent; }
	void ltwChangeNotifyChildren();

	unsigned int childCount() { return children.count; }
	Entity* getChild(unsigned int index);

	HollowSet<Component*>& getRawComponentsSet() { return components; }

private:
	HollowSet<Component*> components;
	SimpleList<Entity*> children;
	Entity* parent = nullptr;
	unsigned int childIndex = 0;

	void addChild(Entity* entity);
	void removeChild(unsigned int index);
	void parentChangeNotifyChildren();

	// Adds a component
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent(T* comp) {
		components.add(comp);

		comp->onStart();
		if (comp->isEnabled()) {
			comp->onEnable();
		}
		return comp;
	}

	void updateTransformPointers();
};

