#include "Presenter.h"

#include "Display/Window.h"
using namespace IonEngine;



void Presenter::initialize(VkInstance instance) {
	if (glfwCreateWindowSurface(instance, window->windowHandle, nullptr, &surface) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create window surface!");
	}
}
