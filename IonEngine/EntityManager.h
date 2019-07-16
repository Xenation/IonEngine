#pragma once
#include "HollowSet.h"
class Entity;
class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void registerEntity(Entity* entity);
	void unregisterEntity(Entity* entity);

	void updateEntities();

	void gui();

private:
	HollowSet<Entity*> entities;

	float updateTimes[300];
	float maxUpdateTime = 0;

	void gui(Entity* entity);
	void addUpdateTime(float time);
};

