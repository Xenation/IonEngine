#include "RenderContext.h"

#include <vector>
#include <string>
#include <fstream>
#include "Core/EngineCore.h"
#include "Display/Window.h"
using namespace IonEngine;



const CappedStringSet<4> validationLayers {
	"VK_LAYER_KHRONOS_validation"
};

const CappedStringSet<4> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	Debug::logWarning(Debug::RENDER, "Validation Layer: %s", pCallbackData->pMessage);

	return VK_FALSE;
}

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		Debug::fail("Failed to open file!");
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}


/* ==== PHYSICAL DEVICE ==== */

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice) : handle(physicalDevice), hasGraphicsQueueFamily(false), hasPresentQueueFamily(false), hasTransferQueueFamily(false) {
	vkGetPhysicalDeviceProperties(handle, &properties);
	vkGetPhysicalDeviceFeatures(handle, &features);
}

bool PhysicalDevice::hasCompleteQueueFamilies() {
	return hasGraphicsQueueFamily && hasPresentQueueFamily && hasTransferQueueFamily;
}

bool PhysicalDevice::isSuitable(VkSurfaceKHR surface) {
	queryQueueFamilies(surface);

	bool extensionSupported = checkExtensionSupport();

	bool swapchainAdequate = false;
	if (extensionSupported) {
		querySwapchainSupport(surface);
		swapchainAdequate = !surfaceFormats.isEmpty() && !presentModes.isEmpty();
	}

	return hasCompleteQueueFamilies() && extensionSupported && swapchainAdequate && features.samplerAnisotropy;
}

void PhysicalDevice::queryQueueFamilies(VkSurfaceKHR surface) {
	CappedDenseSet<VkQueueFamilyProperties, 32> queueFamilies;
	u32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, nullptr);
	queueFamilies.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, queueFamilies.data());

	// Look for graphics and present queue
	i32 i = 0;
	for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			hasGraphicsQueueFamily = true;
			graphicsQueueFamilyIndex = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(handle, i, surface, &presentSupport);
		if (presentSupport) {
			hasPresentQueueFamily = true;
			presentQueueFamilyIndex = i;
		}

		if (hasGraphicsQueueFamily && hasPresentQueueFamily) {
			break;
		}

		i++;
	}

	// Look for transfer only queue
	i = 0;
	for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
		if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0 && (queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == 0) {
			hasTransferQueueFamily = true;
			transferQueueFamilyIndex = i;
			break;
		}

		i++;
	}

	// If no transfer only queue exists, just use the graphics queue
	if (!hasTransferQueueFamily) {
		hasTransferQueueFamily = true;
		transferQueueFamilyIndex = graphicsQueueFamilyIndex;
	}

	// Populate Unique Set
	uniqueQueueFamilyIndices.add(graphicsQueueFamilyIndex);
	if (presentQueueFamilyIndex != graphicsQueueFamilyIndex) {
		uniqueQueueFamilyIndices.add(presentQueueFamilyIndex);
	}
	if (transferQueueFamilyIndex != graphicsQueueFamilyIndex && transferQueueFamilyIndex != presentQueueFamilyIndex) {
		uniqueQueueFamilyIndices.add(transferQueueFamilyIndex);
	}
}

void PhysicalDevice::querySwapchainSupport(VkSurfaceKHR surface) {
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, surface, &surfaceCapabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(handle, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(handle, surface, &formatCount, surfaceFormats.data());
	}

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(handle, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(handle, surface, &presentModeCount, presentModes.data());
	}
}

bool PhysicalDevice::checkExtensionSupport() {
	CappedDenseSet<VkExtensionProperties, 128> availableExtensions;
	u32 extensionCount;
	vkEnumerateDeviceExtensionProperties(handle, nullptr, &extensionCount, nullptr);
	availableExtensions.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(handle, nullptr, &extensionCount, availableExtensions.data());

	CappedStringSet<128> requiredExtensions;
	requiredExtensions.addAll(deviceExtensions.data(), deviceExtensions.getCount());

	for (const VkExtensionProperties& extension : availableExtensions) {
		requiredExtensions.remove(extension.extensionName);
	}

	return requiredExtensions.isEmpty();
}


/* ==== LOGICAL DEVICE ==== */

LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice, bool enableValidation) : physicalDevice(physicalDevice) {
	CappedDenseSet<VkDeviceQueueCreateInfo, 4> queueCreateInfos;

	float queuePriority = 1.0f;
	for (u32 queueFamilyIndex : physicalDevice->uniqueQueueFamilyIndices) {
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndex;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.add(queueInfo);
	}

	VkPhysicalDeviceFeatures enabledDeviceFeatures = {};
	enabledDeviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = queueCreateInfos.getCount();
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.pEnabledFeatures = &enabledDeviceFeatures;
	deviceInfo.enabledExtensionCount = deviceExtensions.getCount();
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidation) {
		deviceInfo.enabledLayerCount = validationLayers.getCount();
		deviceInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		deviceInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice->handle, &deviceInfo, nullptr, &handle) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create logical device!");
	}

	vkGetDeviceQueue(handle, physicalDevice->graphicsQueueFamilyIndex, 0, &graphicsQueue);
	vkGetDeviceQueue(handle, physicalDevice->presentQueueFamilyIndex, 0, &presentQueue);
	vkGetDeviceQueue(handle, physicalDevice->transferQueueFamilyIndex, 0, &transferQueue);
}


/* ==== RENDER CONTEXT ==== */

void RenderContext::initializeInstance() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = engine->appDescription.name;
	appInfo.applicationVersion = VK_MAKE_VERSION(engine->appDescription.versionMajor, engine->appDescription.versionMinor, engine->appDescription.versionPatch);
	appInfo.pEngineName = "IonEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(engine->versionMajor, engine->versionMinor, engine->versionPatch);
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
		populateDebugMessengerCreateInfo(debugMessengerInfo);

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

void RenderContext::setupDebugMessenger() {
	if (!enableValidation) return;

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
	populateDebugMessengerCreateInfo(debugMessengerInfo);

	if (createDebugUtilsMessengerEXT(instance, &debugMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		Debug::fail("Failed to create debug messenger!");
	}
}

void RenderContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugMessengerInfo) {
	debugMessengerInfo = {};
	debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugMessengerInfo.pfnUserCallback = debugCallback;
}

void RenderContext::createPresenter(Window* window) {
	Presenter* presenter = new (presenters.allocate()) Presenter(window);
}

void RenderContext::initializePhysicalDevice() {
	// Query physical devices
	CappedDenseSet<VkPhysicalDevice, 16> physicalDevices;

	u32 physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	if (physicalDeviceCount == 0) {
		Debug::fail(Debug::RENDER, "No physical device detected!");
	}

	physicalDevices.resize(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	// Find a suitable physical device
	for (const VkPhysicalDevice& device : physicalDevices) {
		PhysicalDevice physDevice(device);
		if (physDevice.isSuitable(presenters[0].surface)) {
			physicalDevice = physDevice;
			break;
		}
	}

	if (physicalDevice.handle == VK_NULL_HANDLE) {
		Debug::fail(Debug::RENDER, "Failed to find any suitable physical device!");
	}
}

void RenderContext::initializeLogicalDevice() {
	device = LogicalDevice(&physicalDevice, enableValidation);
}

void RenderContext::createRenderPass() {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = presenters[0].swapchainSurfaceFormat.format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	CappedList<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = attachments.getCount();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device.handle, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create render pass!");
	}
}

VkFormat RenderContext::findDepthFormat() {
	return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat RenderContext::findSupportedFormat(const CappedDenseSet<VkFormat, 32>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice.handle, format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	Debug::fail(Debug::RENDER, "Failed to find a supported format!");
}

void RenderContext::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	CappedList<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = bindings.getCount();
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.handle, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create descriptor set layout!");
	}
}

void RenderContext::createGraphicsPipeline() {
	auto vertShaderCode = readFile("res/shaders/vert.spv");
	auto fragShaderCode = readFile("res/shaders/frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Vertex::getAttributeDescriptions();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchainExtent.width;
	viewport.height = (float) swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		Debug::fail("Failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = nullptr;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		Debug::fail("Failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}
