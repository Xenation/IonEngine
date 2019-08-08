#pragma once
#include "BaseComponentInspector.h"

namespace IonEngine::Editor {
	template<typename Derived, typename Inspected>
	class ComponentInspector : public BaseComponentInspector {
	public:
		static void* initialize() {
			BaseComponentInspector::availableInspectors.add(new Derived());
			return nullptr;
		}

		constexpr ComponentInspector() {
			i = nullptr;
		}

	protected:
		static void* i;

		virtual std::type_index getInspectedType() override {
			return std::type_index(typeid(Inspected));
		}
	};

	template<typename Derived, typename Inspected>
	void* ComponentInspector<Derived, Inspected>::i = ComponentInspector<Derived, Inspected>::initialize();

	#define ION_INSPECTOR(ClassName, InspectedName, Code)\
	class ClassName : public ComponentInspector<ClassName, InspectedName> {\
	Code\
	};\
	template class ComponentInspector<ClassName, InspectedName>;
}
