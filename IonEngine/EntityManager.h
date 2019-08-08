#pragma once
#include "HollowSet.h"

namespace IonEngine {
	class Entity;

	class EntityManager {
	public:
		EntityManager();
		~EntityManager();

		void registerEntity(Entity* entity);
		void unregisterEntity(Entity* entity);

		void updateEntities();

		HollowSet<Entity*>* getEntitiesSet() { return &entities; }
		float* getUpdateTimesPtr() { return updateTimes; }
		unsigned int getUpdateTimesCount() { return 300; }
		float getMaxUpdateTime() { return maxUpdateTime; }

	private:
		HollowSet<Entity*> entities;

		float updateTimes[300];
		float maxUpdateTime = 0;

		void addUpdateTime(float time);
	};
}
