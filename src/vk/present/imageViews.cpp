module;

#include <vulkan/vulkan_core.h>

#include <vector>

export module present:imageViews;

import vkUtils;

std::vector<VkImageView> createImageViews(VkDevice device,
                                          const std::vector<VkImage>& images,
                                          VkFormat imageFormat) {
    std::vector<VkImageView> imageViews(images.size());

    for (size_t i = 0; i < images.size(); i++) {
        VkImageViewCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = imageFormat,
            .components = {VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY},
            .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};

        vkUtils::checkResult(
            vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]));
    }

    return imageViews;
}
