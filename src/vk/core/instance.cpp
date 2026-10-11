module;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <cstring>
#include <print>
#include <vector>

export module core:instance;
import vkUtils;

constexpr std::array validationLayers = {"VK_LAYER_KHRONOS_validation"};

bool checkValidationLayersSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    for (const auto& layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProp : layers) {
            if (std::strcmp(layerName, layerProp.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            std::println("Validation layers are unsupported.");
            return false;
        }
    }

    return true;
}

namespace core {
export VkInstance createInstance(bool isValidationLayersEnabled = false) {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t extCount;
    const char** ext = glfwGetRequiredInstanceExtensions(&extCount);

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = extCount,
        .ppEnabledExtensionNames = ext};

    if (isValidationLayersEnabled && checkValidationLayersSupport()) {
        std::println("Validation layers are enabled");
        createInfo.enabledLayerCount = validationLayers.size();
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    VkInstance instance{};

    vkUtils::checkResult(vkCreateInstance(&createInfo, nullptr, &instance));

    return instance;
}
}  // namespace core
