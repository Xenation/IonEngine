#pragma once
#include "SimpleSet.h"

namespace IonEngine {
	class Light;

	class LightManager {
	public:
		LightManager();
		~LightManager();

		void registerLight(Light* light);
		void unregisterLight(Light* light);

		Light* getMainDirectional();
		SimpleSet<Light*>& getDirectionalLights() { return directionalLights; }
		SimpleSet<Light*>& getPointLights() { return pointLights; }
		SimpleSet<Light*>& getSpotLights() { return spotLights; }

	private:
		SimpleSet<Light*> directionalLights = SimpleSet<Light*>(4, 4);
		SimpleSet<Light*> pointLights = SimpleSet<Light*>(16, 16);
		SimpleSet<Light*> spotLights = SimpleSet<Light*>(16, 16);
	};
}
