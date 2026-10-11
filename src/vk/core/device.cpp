module;

#include <vulkan/vulkan_core.h>

#include <vector>

export module core:device;

import :pdevice;
import vkUtils;

export VkDevice createDevice(VkPhysicalDevice physDevice,
                             QueueFamiliesContext familiesContext) {
    std::vector<const char*> deviceExt = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceVulkan13Features vk13Features{};
    vk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    vk13Features.dynamicRendering = VK_TRUE;
    vk13Features.synchronization2 = VK_TRUE;

    std::vector<uint32_t> uniqueFamilies;
    uniqueFamilies.push_back(familiesContext.graphicsFamily.value());

    if (!familiesContext.isUnifiedQueue()) {
        uniqueFamilies.push_back(familiesContext.presentFamily.value());
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    float pripority = 1.0f;
    for (const auto& family : uniqueFamilies) {
        VkDeviceQueueCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &pripority};

        queueCreateInfos.push_back(createInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = &vk13Features;
    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExt.size());
    createInfo.ppEnabledExtensionNames = deviceExt.data();

    VkDevice device;

    vkUtils::checkResult(
        vkCreateDevice(physDevice, &createInfo, nullptr, &device));

    return device;
}
