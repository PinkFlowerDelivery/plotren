module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <optional>

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
    CoreContext(GLFWwindow* window) {
        instance_ = createInstance(true);
        surface_ = createSurface(instance_, window);
        physDeviceContext_ = pickPhysicalDevice(instance_, surface_);
        device_ = createDevice(physDeviceContext_.physDevice,
                               physDeviceContext_.familiesContext);
    }

    ~CoreContext() {
        vkDestroyDevice(device_, nullptr);
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        vkDestroyInstance(instance_, nullptr);
    }

    [[nodiscard]] VkSurfaceKHR getSurface() const { return surface_; }

    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const {
        return physDeviceContext_.physDevice;
    }
    [[nodiscard]] QueueFamiliesContext getQueueFamiliesContext() const {
        return physDeviceContext_.familiesContext;
    }
    [[nodiscard]] std::optional<uint32_t> getGraphicFamily() const {
        return physDeviceContext_.familiesContext.graphicsFamily;
    }
    [[nodiscard]] std::optional<uint32_t> getPresentFamily() const {
        return physDeviceContext_.familiesContext.presentFamily;
    }
    [[nodiscard]] VkDevice getDevice() const { return device_; }
};
}  // namespace core
