#pragma once

#include "SimpleList.h"

class Light;

class LightManager {
public:
	LightManager();
	~LightManager();

	void registerLight(Light* light);
	void unregisterLight(Light* light);

	Light* getMainDirectional();

private:
	SimpleList<Light*> directionalLights = SimpleList<Light*>(4, 4);
	SimpleList<Light*> pointLights = SimpleList<Light*>(16, 16);
	SimpleList<Light*> spotLights = SimpleList<Light*>(16, 16);
};

