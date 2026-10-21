module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <cstdint>
#include <vector>

export module present:swapchain;

import vkUtils;
import core;

export struct SwapchainContext {
    VkSwapchainKHR swapchain;
    VkFormat format;
};

VkSurfaceFormatKHR chooseSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats,
    VkFormat targetFormat, VkColorSpaceKHR targetColorSpace) {
    for (const auto format : availableFormats) {
        if (format.format == targetFormat &&
            format.colorSpace == targetColorSpace) {
            return format;
        }
    }

    return availableFormats[0];
}

VkExtent2D chooseExtent(GLFWwindow* window,
                        VkSurfaceCapabilitiesKHR capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int32_t width, height;

    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);

    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
}

export SwapchainContext createSwapchain(GLFWwindow* window,
                                        vk::CoreContext coreContext) {
    SwapchainContext context;

    uint32_t formatCount;
    vkUtils::checkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(
        coreContext.getPhysicalDevice(), coreContext.getSurface(), &formatCount,
        nullptr));

    std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
    vkUtils::checkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(
        coreContext.getPhysicalDevice(), coreContext.getSurface(), &formatCount,
        availableFormats.data()));

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkUtils::checkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        coreContext.getPhysicalDevice(), coreContext.getSurface(),
        &surfaceCapabilities));

    VkSurfaceFormatKHR surfaceFormat =
        chooseSurfaceFormat(availableFormats, VK_FORMAT_R8G8B8A8_SRGB,
                            VK_COLORSPACE_SRGB_NONLINEAR_KHR);

    context.format = surfaceFormat.format;

    std::vector<uint32_t> familyIndices;
    if (coreContext.getQueueFamiliesContext().isUnifiedQueue()) {
        familyIndices.push_back(coreContext.getGraphicFamily().value());
    } else {
        familyIndices.push_back(coreContext.getGraphicFamily().value());
        familyIndices.push_back(coreContext.getPresentFamily().value());
    }

    VkSwapchainCreateInfoKHR createInfo{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = coreContext.getSurface(),
        .minImageCount = surfaceCapabilities.minImageCount + 1,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = chooseExtent(window, surfaceCapabilities),
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = static_cast<uint32_t>(familyIndices.size()),
        .pQueueFamilyIndices = familyIndices.data(),
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE,
    };

    vkUtils::checkResult(vkCreateSwapchainKHR(
        coreContext.getDevice(), &createInfo, nullptr, &context.swapchain));

    return context;
};
