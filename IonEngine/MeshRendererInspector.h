#pragma once
#include "ComponentInspector.h"
#include "MeshRenderer.h"

ION_INSPECTOR(MeshRendererInspector, MeshRenderer,
public:
	virtual void drawGui(Component* component) override;
)

