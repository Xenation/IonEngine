#include "RenderContext.h"

using namespace IonEngine;



const CappedStringSet<4> validationLayers {
	"VK_LAYER_KHRONOS_validation"
};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	Debug::logWarning(Debug::RENDER, "Validation Layer: %s", pCallbackData->pMessage);

	return VK_FALSE;
}

void RenderContext::initialize() {

}

void RenderContext::initializeInstance() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "IonEngine Test"; // TODO make customizable
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName = "IonEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	CappedStringSet<64> requiredExtensions;
	populateRequiredExtensions(requiredExtensions);
	instanceInfo.enabledExtensionCount = requiredExtensions.getCount();
	instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (enableValidation) {
		instanceInfo.enabledLayerCount = validationLayers.getCount();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = debugCallback;

		instanceInfo.pNext = &debugMessengerInfo;
	} else {
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;
	}

	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
		Debug::fail("Failed to create vulkan instance!");
	}
}

void RenderContext::populateRequiredExtensions(CappedStringSet<64>& requiredExtensions) {
	u32 glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	requiredExtensions.addAll(glfwExtensions, glfwExtensionCount);

	if (enableValidation) {
		requiredExtensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}
