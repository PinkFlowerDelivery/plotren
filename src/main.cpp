#include <vulkan/vulkan_core.h>

#include <format>
#include <print>

import core;
import window;

int main() {
    Window window{640, 480, "plotren"};

    core::CoreContext ctx{window.getWindow()};
    std::println("Hello, world!");
    return 0;
}
