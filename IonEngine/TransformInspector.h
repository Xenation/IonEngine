#pragma once
#include "ComponentInspector.h"
#include "Transform.h"

ION_INSPECTOR(TransformInspector, Transform, 
public:
	virtual void drawGui(Component* component) override;
)

