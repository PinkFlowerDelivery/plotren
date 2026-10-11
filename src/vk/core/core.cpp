module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

export module core;

import :instance;
import :pdevice;
import :surface;
import :device;

namespace core {
export class CoreContext {
    VkInstance instance_;
    VkSurfaceKHR surface_;
    PhysicalDeviceContext physDeviceContext_;
    VkDevice device_;

   public:
    CoreContext(GLFWwindow* window) : instance_(createInstance(true)) {
        surface_ = createSurface(instance_, window);
        physDeviceContext_ = pickPhysicalDevice(instance_, surface_);
        device_ = createDevice(physDeviceContext_.physDevice,
                               physDeviceContext_.familiesContext);
    }
};
}  // namespace core
