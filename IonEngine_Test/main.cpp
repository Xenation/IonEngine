#include <iostream>
#include <Core/EngineCore.h>
#include <Core/Module.h>
#include <Core/Collections/BulkSparseSet.h>

using namespace IonEngine;


struct TestSt {
	u8 a;
	u8 b;
	u16 c;
	u32 d;
};

class ModuleA : public Module {
public:
	ModuleA(EngineCore* core) : Module(core) {
		Debug::logInfo("ModuleA Init");
	}
	virtual ~ModuleA() {
		Debug::logInfo("ModuleA Destroy");
	}
};

class ModuleB : public Module {
public:
	ModuleB(EngineCore* core) : Module(core) {
		Debug::logInfo("ModuleB Init");
	}
	virtual ~ModuleB() {
		Debug::logInfo("ModuleB Destroy");
	}
};

int main(int argc, char** args) {

	EngineCore* core = new EngineCore();
	core->moduleManager.addModule<ModuleA>();
	core->moduleManager.addModule<ModuleB>();

	delete core;

	return 0;
}
