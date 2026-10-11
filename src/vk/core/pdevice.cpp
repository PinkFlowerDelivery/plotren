
module;

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <optional>
#include <print>
#include <string_view>
#include <vector>

export module core:pdevice;

struct QueueFamiliesContext {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isUnifiedQueue() const {
        return graphicsFamily.has_value() && presentFamily.has_value() &&
               graphicsFamily.value() == presentFamily.value();
    }

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct PhysicalDeviceContext {
    VkPhysicalDevice physDevice = VK_NULL_HANDLE;
    QueueFamiliesContext familiesContext;
};

int32_t rateDevice(VkPhysicalDevice pDevice) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(pDevice, &props);

    int32_t score = 0;

    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 100;
    }

    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        score += 10;
    }

    return score;
}

bool isDeviceSuitable(VkPhysicalDevice pDevice) {
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(pDevice, &features);

    uint32_t extCount;
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extCount, nullptr);

    std::vector<VkExtensionProperties> extProps(extCount);
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extCount,
                                         extProps.data());

    bool isDynamicRenderingSupport = false;
    for (const auto& extProp : extProps) {
        if (std::string_view(extProp.extensionName) ==
            VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) {
            isDynamicRenderingSupport = true;
            break;
        }
    }

    if (!isDynamicRenderingSupport) {
        return false;
    }

    return true;
}

QueueFamiliesContext findQueueFamilies(VkPhysicalDevice pDevice,
                                       VkSurfaceKHR surface) {
    QueueFamiliesContext context;

    uint32_t familiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &familiesCount, nullptr);

    std::vector<VkQueueFamilyProperties> familiesProps(familiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &familiesCount,
                                             familiesProps.data());

    for (size_t i = 0; i < familiesProps.size(); i++) {
        if (familiesProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            context.graphicsFamily = i;
        }

        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface,
                                             &presentSupport);

        if (presentSupport) {
            context.presentFamily = i;
        }

        if (context.isComplete()) {
            return context;
        }
    }

    return context;
}

export PhysicalDeviceContext pickPhysicalDevice(VkInstance instance,
                                                VkSurfaceKHR surface) {
    uint32_t pDeviceCount;
    vkEnumeratePhysicalDevices(instance, &pDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> pDevices(pDeviceCount);
    vkEnumeratePhysicalDevices(instance, &pDeviceCount, pDevices.data());

    int32_t currentScore = 0;
    QueueFamiliesContext bestQueueFamilies;
    VkPhysicalDevice bestPhysDevice = VK_NULL_HANDLE;

    for (const auto& device : pDevices) {
        int32_t score = rateDevice(device);
        auto queueFamilies = findQueueFamilies(device, surface);

        if (!isDeviceSuitable(device)) {
            continue;
        }

        if (score > currentScore) {
            currentScore = score;
            bestQueueFamilies = queueFamilies;
            bestPhysDevice = device;
        }
    }

    std::println(
        "Queue families: \n  Graphic: {}\n  Present: {}\n  Is unified: {}",
        bestQueueFamilies.graphicsFamily.has_value(),
        bestQueueFamilies.presentFamily.has_value(),
        bestQueueFamilies.isUnifiedQueue());

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(bestPhysDevice, &deviceProperties);
    std::println("Selected gpu: {}", deviceProperties.deviceName);

    return {.physDevice = bestPhysDevice, .familiesContext = bestQueueFamilies};
}
