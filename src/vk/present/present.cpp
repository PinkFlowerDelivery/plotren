
module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <vector>

export module present;

import :swapchain;
import :images;
import :imageViews;

namespace vk {

export class Present {
    SwapchainContext swapchainContext_;
    std::vector<VkImage> images_;
    std::vector<VkImageView> imageViews_;

   public:
    Present(GLFWwindow* window, core::CoreContext coreCtx) {
        swapchainContext_ = createSwapchain(window, coreCtx);
        images_ = getSwapchainImages(coreCtx.getDevice(),
                                     swapchainContext_.swapchain);
        imageViews_ = createImageViews(coreCtx.getDevice(), images_,
                                       swapchainContext_.format);
    }
};
}  // namespace vk
