#include "WindowModule.h"
#include "Window.h"
#include "Core/Collections/CappedSparseSet.h"

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

	windows = new CappedSparseSet<Window, 16>();
}

WindowModule::~WindowModule() {
	for (Window& window : *windows) {
		window.~Window();
	}
	delete windows;
	glfwTerminate();
}


Window* WindowModule::createWindow(const char* title) {
	return new (windows->allocate()) Window(title);
}

Window* WindowModule::createWindow(const char* title, WindowState state) {
	return new (windows->allocate()) Window(title, state);
}

Window* WindowModule::createWindow(const char* title, Vec2i pos, Vec2i size, WindowState state) {
	return new (windows->allocate()) Window(title, pos, size, state);
}

void WindowModule::destroyWindow(Window* window) {
	window->~Window();
	windows->remove(window);
}

void WindowModule::poolEvents() {
	glfwPollEvents();
}
