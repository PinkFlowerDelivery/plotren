module;

#include <GLFW/glfw3.h>

#include <stdexcept>

export module window;

export class Window {
    GLFWwindow* window_;

   public:
    Window(int32_t w, int32_t h, const char* title) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window_ = glfwCreateWindow(w, h, title, nullptr, nullptr);
    }

    ~Window() {
        if (window_) {
            glfwDestroyWindow(window_);
        }

        glfwTerminate();
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    [[nodiscard]] GLFWwindow* getWindow() const { return window_; }
};
