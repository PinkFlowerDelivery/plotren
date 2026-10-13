
module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

export module present;

import :swapchain;

namespace vk {

export class Present {
    VkSwapchainKHR swapchain_;

   public:
    Present(GLFWwindow* window, core::CoreContext coreCtx) {
        swapchain_ = createSwapchain(window, coreCtx);
    }
};
}  // namespace vk
