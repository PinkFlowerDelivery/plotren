module;

#include <vulkan/vulkan_core.h>

#include <format>
#include <source_location>

export module vkUtils;

namespace vkUtils {

export inline void checkResult(
    VkResult res, std::source_location loc = std::source_location::current()) {
    if (res != VK_SUCCESS) {
        throw std::runtime_error(std::format("Vulkan error: {} at {}:{}",
                                             static_cast<int32_t>(res),
                                             loc.file_name(), loc.line()));
    }
}

}  // namespace vkUtils
