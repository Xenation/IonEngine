#pragma once
#include "ComponentInspector.h"
#include "Camera.h"

namespace IonEngine::Editor {
	ION_INSPECTOR(CameraInspector, Camera,
	public:
		virtual void drawGui(Component* component) override;
	)
}
