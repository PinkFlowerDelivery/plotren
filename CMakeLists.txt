
cmake_minimum_required(VERSION 3.28)

project(plotren VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(source src/main.cpp)
add_executable(plotren ${source})

target_include_directories(plotren PRIVATE src)
target_link_libraries(plotren Vulkan::Vulkan glfw glm::glm)
