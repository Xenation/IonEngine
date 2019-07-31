#pragma once
#include "ComponentInspector.h"
#include "Camera.h"

ION_INSPECTOR(CameraInspector, Camera,
public:
	virtual void drawGui(Component* component) override;
)
