#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Core/Collections/CappedStringSet.h"

namespace IonEngine {
	class RenderContext {
	public:
		RenderContext(const RenderContext&) = delete;
		void operator=(const RenderContext&) = delete;

		void initialize();

	private:
		bool enableValidation;
		VkInstance instance;
		
		VkPhysicalDevice physicalDevice;
		VkDevice device;

		void initializeInstance();
		void populateRequiredExtensions(CappedStringSet<64>& requiredExtensions);
	};
}
