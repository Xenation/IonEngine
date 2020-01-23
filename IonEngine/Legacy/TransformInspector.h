#pragma once
#include "ComponentInspector.h"
#include "Transform.h"

namespace IonEngine::Editor {
	ION_INSPECTOR(TransformInspector, Transform,
	public:
		virtual void drawGui(Component* component) override;
	)
}
