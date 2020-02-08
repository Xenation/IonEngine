#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Core/Collections/CappedStringSet.h"
#include "Core/Collections/CappedDenseSet.h"
#include "Presenter.h"

namespace IonEngine {
	class EngineCore;

	struct PhysicalDevice {
		VkPhysicalDevice handle;
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;

		bool hasGraphicsQueueFamily;
		u32 graphicsQueueFamilyIndex;
		bool hasPresentQueueFamily;
		u32 presentQueueFamilyIndex;
		bool hasTransferQueueFamily;
		u32 transferQueueFamilyIndex;
		CappedDenseSet<u32, 4> uniqueQueueFamilyIndices;

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		CappedDenseSet<VkSurfaceFormatKHR, 64> surfaceFormats;
		CappedDenseSet<VkPresentModeKHR, 16> presentModes;

		PhysicalDevice() {}
		PhysicalDevice(VkPhysicalDevice physicalDevice);

		bool hasCompleteQueueFamilies();
		bool isSuitable(VkSurfaceKHR surface);

	private:
		bool checkExtensionSupport();
		void queryQueueFamilies(VkSurfaceKHR surface);
		void querySwapchainSupport(VkSurfaceKHR surface);
	};

	struct LogicalDevice {
		VkDevice handle;
		PhysicalDevice* physicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue transferQueue;

		LogicalDevice() {}
		LogicalDevice(PhysicalDevice* physicalDevice, bool enableValidation);
	};

	class RenderContext {
	public:
		RenderContext(EngineCore* core) : engine(core) {}
		RenderContext(const RenderContext&) = delete;
		void operator=(const RenderContext&) = delete;

		void initializeInstance();
		void createPresenter(Window* window);
		void initializeContext();


	private:
		EngineCore* engine;
		CappedDenseSet<Presenter, 32> presenters;

		bool enableValidation;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		
		PhysicalDevice physicalDevice;
		LogicalDevice device;

		VkRenderPass renderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		void populateRequiredExtensions(CappedStringSet<64>& requiredExtensions);
		void setupDebugMessenger();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugMessengerInfo);

		void initializePhysicalDevice();
		void initializeLogicalDevice();

		void createRenderPass();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const CappedDenseSet<VkFormat, 32>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		void createDescriptorSetLayout();

		void createGraphicsPipeline();
	};
}
