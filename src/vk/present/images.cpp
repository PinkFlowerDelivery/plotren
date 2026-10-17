module;

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

export module present:images;

import vkUtils;

std::vector<VkImage> getSwapchainImages(VkDevice device,
                                        VkSwapchainKHR swapchain) {
    uint32_t imageCount;
    vkUtils::checkResult(
        vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));

    std::vector<VkImage> images(imageCount);

    vkUtils::checkResult(
        vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

    return images;
}
