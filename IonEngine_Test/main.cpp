#include <iostream>
#include <Core/EngineCore.h>
#include <Core/Module.h>
#include <Display/Window.h>
#include <Display/WindowModule.h>
#include <Render/RenderModule.h>
#include <Core/Collections/BulkSparseSet.h>


using namespace IonEngine;


int main(int argc, char** args) {

	EngineCore* core = new EngineCore();
	WindowModule* windows = core->moduleManager.addModule<WindowModule>();
	RenderModule* render = core->moduleManager.addModule<RenderModule>();
	Window* window = windows->createWindow("Test", Vec2i::one * 50, Vec2i(1280, 720), WindowState::WINDOWED);
	render->initialize(window);

	while (!window->shouldClose()) {
		windows->poolEvents();
		render->render();
	}

	delete core;

	return 0;
}
