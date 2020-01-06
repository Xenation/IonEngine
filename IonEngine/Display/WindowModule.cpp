#include "WindowModule.h"
#include "Window.h"
#include "../Core/Collections/BulkHollowSet.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
using namespace IonEngine;



void glfwErrorCallback(i32 error, const char* description) {
	Debug::logError(Debug::DISPLAY, "GLFW ERROR (%d): %s", description);
}

WindowModule::WindowModule(EngineCore* engine) : Module(engine) {
	glfwSetErrorCallback(glfwErrorCallback);

	if (glfwInit()) {
		Debug::logInfo(Debug::DISPLAY, "Initialized GLFW!");
	} else {
		Debug::fail(Debug::DISPLAY, "Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // TODO set conditionally

	windows = new BulkHollowSet<Window>();
}

WindowModule::~WindowModule() {
	// TODO iterate through the entire set to manually delete all windows
	delete windows;
	glfwTerminate();
}


Window* WindowModule::createWindow(const char* title) {
	return windows->add(Window(title));
}

Window* WindowModule::createWindow(const char* title, WindowState state) {
	return windows->add(Window(title, state));
}

Window* WindowModule::createWindow(const char* title, Vec2i pos, Vec2i size, WindowState state) {
	return windows->add(Window(title, pos, size, state));
}

void WindowModule::poolEvents() {
	glfwPollEvents();
}
