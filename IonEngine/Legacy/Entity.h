#pragma once
#include <type_traits>
#include <typeinfo>
#include "Component.h"
#include "HollowSet.h"
#include "SimpleList.h"
#include "Transform.h"

namespace IonEngine {
	class Entity {
	public:
		typedef u32 Id;
		Id id;
		std::string name;
		Transform* transform = nullptr;
		bool isDying = false;

		Entity(const char* name = "Entity", bool hasTransform = true);
		Entity(std::string name = "Entity", bool hasTransform = true);
		Entity(const Entity&) = delete;
		~Entity();

		// Adds a component by creating a new
		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T* addComponent() {
			T* nComponent = new T(this);
			return addComponent<T>(nComponent);
		}

		// Adds a component from an existing instance (this instance must not have already been assigned to another entity)
		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T* addComponent(T* comp) {
			components.add(comp);

			if (typeid(T) == typeid(Transform)) {
				replaceTransformPointers((Transform*) comp);
			}

			comp->onStart();
			if (comp->isEnabled()) {
				comp->onEnable();
			}
			return comp;
		}

		// Gets a component of the given type
		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T* getComponent() {
			for (u32 i = 0; i < components.capacity; i++) {
				if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) return (T*) components[i];
			}
			return nullptr;
		}

		// Gets all the components of the given type
		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T** getComponents() {
			u32 count = 0;
			for (u32 i = 0; i < components.capacity; i++) {
				if (count == components.count) break;
				if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) count++;
			}
			T** comps = new Component*[count];
			getComponents(comps, count);
			return comps;
		}

		// Gets all the components of the given type using a preallocated array, returns the count of components found
		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		u32 getComponents(T** arr, u32 arrSize) {
			u32 arrIndex = 0;
			for (u32 i = 0; i < components.capacity && arrIndex < arrSize; i++) {
				if (arrIndex == components.count) break;
				if (components[i] != nullptr && typeid(*components[i]) == typeid(T)) {
					arr[arrIndex++] = components[i];
				}
			}
			return arrIndex;
		}

		// Removes a previously added component
		void removeComponent(Component* comp) {
			for (u32 i = 0; i < components.count; i++) {
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

		u32 childCount() { return children.count; }
		Entity* getChild(u32 index);

		const HollowSet<Component*>& getRawComponentsSet() const { return components; }
		HollowSet<Component*>& getRawComponentsSet() { return components; }

	private:
		HollowSet<Component*> components;
		SimpleList<Entity*> children;
		Entity* parent = nullptr;
		u32 childIndex = 0;

		void addChild(Entity* entity);
		void removeChild(u32 index);
		void parentChangeNotifyChildren();

		void replaceTransformPointers(Transform* nTransform);
	};
}
