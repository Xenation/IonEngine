#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace IonEngine {
	class Window;
	class RenderContext;

	class Presenter {
	public:
		Presenter(Window* window) : window(window) {}
		Presenter(const Presenter&) = delete;
		void operator=(const Presenter&) = delete;

		void initialize(VkInstance instance);

	private:
		Window* window;
		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
	};
}
