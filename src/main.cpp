#include <vulkan/vulkan_core.h>

#include <format>
#include <print>

import core;
import window;
import present;

int main() {
    Window window{640, 480, "plotren"};

    core::CoreContext ctx{window.getWindow()};
    vk::Present present{window.getWindow(), ctx};
    std::println("Hello, world!");
    return 0;
}
