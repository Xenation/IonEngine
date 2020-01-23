#pragma once
#include <typeindex>
#include "SimpleSet.h"

namespace IonEngine {
	class Component;
}

namespace IonEngine::Editor {
	class BaseComponentInspector {
	public:
		static SimpleSet<BaseComponentInspector*> availableInspectors;

		static BaseComponentInspector* getInspector(std::type_index type) {
			for (u32 i = 0; i < availableInspectors.count; i++) {
				if (availableInspectors[i]->getInspectedType() == type) {
					return availableInspectors[i]; // TODO switch to having one inspector instance per component for more flexibility
				}
			}
			return nullptr;
		}

		virtual void drawGui(Component* component) = 0;

	protected:
		virtual std::type_index getInspectedType() = 0;
	};
}
