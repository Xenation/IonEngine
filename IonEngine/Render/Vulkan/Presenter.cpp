#if ION_VULKAN
#include "Presenter.h"

#include "Display/Window.h"
#include "RenderContext.h"
using namespace IonEngine;



void Presenter::initialize(VkInstance instance) {
	if (glfwCreateWindowSurface(instance, window->windowHandle, nullptr, &surface) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create window surface!");
	}
}

void Presenter::createSwapchain(LogicalDevice* device) {
	
	// Choose Surface Format
	swapchainSurfaceFormat = device->physicalDevice->surfaceFormats[0];
	for (const VkSurfaceFormatKHR& availableFormat : device->physicalDevice->surfaceFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			swapchainSurfaceFormat = availableFormat;
			break;
		}
	}

	// Choose Present Mode
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const VkPresentModeKHR& availablePresentMode : device->physicalDevice->presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
			break;
		}
	}

	// Choose Swap extent
	VkSurfaceCapabilitiesKHR& surfaceCapabilities = device->physicalDevice->surfaceCapabilities;
	swapchainExtent = surfaceCapabilities.currentExtent;
	if (swapchainExtent.width == UINT32_MAX) {
		Vec2i extent = window->getFramebufferSize();
		swapchainExtent = {static_cast<u32>(extent.x), static_cast<u32>(extent.y)};

		swapchainExtent.width = clamp(swapchainExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		swapchainExtent.height = clamp(swapchainExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}

	// Determine image count
	u32 imageCount = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
		imageCount = surfaceCapabilities.maxImageCount;
	}

	// Create Swapchain
	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = surface;
	swapchainInfo.minImageCount = imageCount;
	swapchainInfo.imageFormat = swapchainSurfaceFormat.format;
	swapchainInfo.imageColorSpace = swapchainSurfaceFormat.colorSpace;
	swapchainInfo.imageExtent = swapchainExtent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	u32 queueFamilyIndices[] = {device->physicalDevice->graphicsQueueFamilyIndex, device->physicalDevice->presentQueueFamilyIndex};
	if (device->physicalDevice->graphicsQueueFamilyIndex != device->physicalDevice->presentQueueFamilyIndex) {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = nullptr;
	}

	swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device->handle, &swapchainInfo, nullptr, &swapchain) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create swapchain!");
	}

	vkGetSwapchainImagesKHR(device->handle, swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->handle, swapchain, &imageCount, swapchainImages.data());
}

void Presenter::createImageViews(LogicalDevice* device) {
	swapchainImageViews.resize(swapchainImages.getCount());
	for (size_t i = 0; i < swapchainImages.getCount(); i++) {
		swapchainImageViews[i] = createImageView(device, swapchainImages[i], swapchainSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

VkImageView Presenter::createImageView(LogicalDevice* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
	VkImageViewCreateInfo imageViewInfo = {};
	imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewInfo.image = image;
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewInfo.format = format;
	imageViewInfo.subresourceRange.aspectMask = aspectFlags;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device->handle, &imageViewInfo, nullptr, &imageView) != VK_SUCCESS) {
		Debug::fail(Debug::RENDER, "Failed to create image view!");
	}

	return imageView;
}

#endif