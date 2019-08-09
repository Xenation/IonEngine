#pragma once
#include "ComponentInspector.h"
#include "Light.h"

namespace IonEngine::Editor {
	ION_INSPECTOR(LightInspector, Light,
	public:
		virtual void drawGui(Component* component) override;
	)
}
