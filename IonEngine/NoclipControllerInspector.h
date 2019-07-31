#pragma once
#include "ComponentInspector.h"
#include "NoclipController.h"

ION_INSPECTOR(NoclipControllerInspector, NoclipController,
public:
	virtual void drawGui(Component* component) override;
)

