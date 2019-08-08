#pragma once
#include "ComponentInspector.h"
#include "NoclipController.h"

namespace IonEngine::Editor {
	ION_INSPECTOR(NoclipControllerInspector, NoclipController,
	public:
		virtual void drawGui(Component* component) override;
	)
}
