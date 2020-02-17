#pragma once
#include "Core/Collections/BulkSparseSet.h"
#include "Core/Collections/BulkSparseDataSet.h"
#include "Core/Collections/CappedSparseArray.h"

namespace IonEngine {
	typedef u64 ComponentMask;
	typedef u16 ComponentId;

	struct Entity {
		ComponentMask componentMask;

		Entity(const Entity&) = delete;
		void operator=(const Entity&) = delete;
	};

	struct ComponentPool {
	private:
		/*struct ComponentData {
			Entity* entity;
			u8 dataStart;
		};*/

		template<typename T>
		struct ComponentData {
			static constexpr u16 dataSize = sizeof(T);
			u8 data[sizeof(T)];
		};

	public:
		template<typename T>
		T* allocateComponent(Entity* entity) {
			T* component = reinterpret_cast<T*>(componentData->allocate());
			*entityPointers.allocate() = entity;
			return component;
		}

		void freeComponent(Entity* entity) {
			u32 index = 0;
			for (Entity* entityPtr : entityPointers) {
				if (entityPtr == entity) {
					break;
				}
				index++;
			}
			componentData.removeAt(index);
			entityPointers.removeAt(index);
		}

	private:
		BulkSparseDataSet componentData;
		BulkSparseSet<Entity*> entityPointers;

		friend class Scene;
	};

	class Scene {
	public:
		Entity* createEntity();
		void destroyEntity(Entity* entity);

		template<typename T>
		T* assignComponent(Entity* entity) {
			ComponentId componentId = getComponentId<T>();

			ComponentPool* pool;
			if (!componentPools.exists(componentId)) {
				pool = new (componentPools.use(componentId)) ComponentPool(new BulkSparseSet<T>());
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

			pool->freeComponent<sizeof(T)>(entity);

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


