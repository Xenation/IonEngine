#include "RenderModule.h"

#if ION_VULKAN
#include "Vulkan/RenderContext.h"
#else
#include "DX11/RenderContext.h"
#endif
#include "Display/Window.h"

using namespace IonEngine;



RenderModule::RenderModule(EngineCore* engine) : Module(engine) {
	context = new RenderContext(engine);
}

RenderModule::~RenderModule() {
	context->shutdown();
	delete context;
}


void RenderModule::initialize(Window* window) {
	Vec2i size = window->getSize();
	HWND win32Handle;
	void* win32ptr = reinterpret_cast<void*>(&win32Handle);
	window->getWin32Handle(win32ptr);
	context->initialize(size.x, size.y, true, win32Handle, false, 0.1f, 1000.0f);
}

void RenderModule::render() {
	context->beginFrame(Color::green);
	context->endFrame();
}
