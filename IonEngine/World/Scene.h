#pragma once
#include "Core/Collections/BulkSparseSet.h"
#include "Core/Collections/CappedSparseArray.h"

namespace IonEngine {
	typedef u64 ComponentMask;
	typedef u16 ComponentId;

	struct Entity {
		ComponentMask componentMask;
	};

	//template<typename T>
	struct ComponentPool {
	private:
		struct ComponentData {
			Entity* entity;
			u8 dataStart;
		};

	public:
		template<typename T>
		ComponentPool() {
			data = reinterpret_cast<u8*>(new BulkSparseSet<T>());
		}

		template<typename T>
		T* allocateComponent(Entity* entity) {
			BulkSparseSet<T>* components = reinterpret_cast<BulkSparseSet<T>>(data);
			T* component = components->allocate();
			*entityPointers.allocate() = entity;
			return component;
		}

		template<typename T>
		void freeComponent(Entity* entity) {
			BulkSparseSet<T>* components = reinterpret_cast<BulkSparseSet<T>>(data);
			u32 index = 0;
			for (Entity* entityPtr : entityPointers) {
				if (entityPtr == entity) {
					break;
				}
				index++;
			}
			components->removeAt(index);
			entityPointers.removeAt(index);
		}

	private:
		u8* data;
		BulkSparseSet<Entity*> entityPointers;
	};

	class Scene {
	public:
		Entity* createEntity();
		void destroyEntity(Entity* entity); // TODO continue here

		template<typename T>
		T* assignComponent(Entity* entity) {
			ComponentId componentId = getComponentId<T>();

			ComponentPool* pool;
			if (!componentPools.exists(componentId)) {
				pool = new (componentPools.use(componentId)) ComponentPool<T>();
			} else {
				pool = componentPools.at(componentId);
			}

			T* component = pool->allocateComponent<T>(entity);

			entity->componentMask |= 1 << getComponentId<T>();
			return component;
		}
		template<typename T>
		void unassignComponent(Entity* entity) {
			ComponentId componentId = getComponentId<T>();

			ComponentPool* pool;
			if (componentPools.exists(componentId)) {
				pool = componentPools.at(componentId);
			} else {
				// The pool does not exist there is nothing to delete
				return;
			}

			pool->freeComponent(entity);

			entity->componentMask &= ~(1 << getComponentId<T>());
		}

	private:
		BulkSparseSet<Entity> entities;
		CappedSparseArray<ComponentPool, 64> componentPools;

	private:
		static ComponentId componentCounter;
		template<typename T>
		static ComponentId getComponentId() {
			static ComponentId id = componentCounter++;
			return id;
		}
	};
}
