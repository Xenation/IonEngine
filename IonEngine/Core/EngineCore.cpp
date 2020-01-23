#include "EngineCore.h"

#include "Module.h"
using namespace IonEngine;



ModuleManager::~ModuleManager() {
	for (Module* mod : modules) {
		delete mod;
	}
}
