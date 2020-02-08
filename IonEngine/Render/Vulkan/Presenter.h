#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Core/Collections/CappedList.h"

namespace IonEngine {
	class Window;
	class RenderContext;
	class LogicalDevice;

	class Presenter {
	public:
		Presenter(const Presenter&) = delete;
		void operator=(const Presenter&) = delete;

		void initialize(VkInstance instance);
		void createSwapchain(LogicalDevice* device);
		void createImageViews(LogicalDevice* device);

	private:
		Window* window;
		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
		VkSurfaceFormatKHR swapchainSurfaceFormat;
		VkExtent2D swapchainExtent;
		CappedList<VkImage, 16> swapchainImages;
		CappedList<VkImageView, 16> swapchainImageViews;

		friend class RenderContext;

		Presenter(Window* window) : window(window) {}

		VkImageView createImageView(LogicalDevice* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	};
}
