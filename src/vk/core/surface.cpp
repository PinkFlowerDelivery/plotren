module;

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <stdexcept>

export module core:surface;

import vkUtils;

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window) {
    if (!glfwVulkanSupported()) {
        throw std::runtime_error("Vulkan is unavailable.");
    }

    VkSurfaceKHR surface;

    vkUtils::checkResult(
        glfwCreateWindowSurface(instance, window, nullptr, &surface));

    return surface;
}
