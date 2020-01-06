#include "EngineCore.h"

using namespace IonEngine;



ModuleManager::~ModuleManager() {
	for (u32 i = 0; i < modules.count; i++) {
		delete modules[i];
	}
}
