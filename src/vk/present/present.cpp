
module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <vector>

export module present;

import :swapchain;
import :images;
import :imageViews;
import core;

namespace vk {

export class Present {
    // We store the device here for cleanup in the dtor
    VkDevice device_;

    SwapchainContext swapchainContext_;
    std::vector<VkImage> images_;
    std::vector<VkImageView> imageViews_;

   public:
    Present(GLFWwindow* window, vk::CoreContext coreCtx) {
        swapchainContext_ = createSwapchain(window, coreCtx);
        images_ = getSwapchainImages(coreCtx.getDevice(),
                                     swapchainContext_.swapchain);
        imageViews_ = createImageViews(coreCtx.getDevice(), images_,
                                       swapchainContext_.format);
    }
    ~Present() {
        for (const auto& imageView : imageViews_) {
            vkDestroyImageView(device_, imageView, nullptr);
        }

        for (const auto& image : images_) {
            vkDestroyImage(device_, image, nullptr);
        }

        vkDestroySwapchainKHR(device_, swapchainContext_.swapchain, nullptr);
    }
};
}  // namespace vk
