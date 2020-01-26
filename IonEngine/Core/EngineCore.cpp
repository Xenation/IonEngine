#include "EngineCore.h"

#include "Module.h"
using namespace IonEngine;



ModuleManager::~ModuleManager() {
	// Delete the modules in reverse order of creation
	for (i32 i = static_cast<i32>(modules.getCount()) - 1; i >= 0; i--) {
		delete modules[i];
	}
}
