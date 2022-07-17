// working from https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Base_code

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

#include "vulkan.h"

using namespace std::string_literals;

class HelloTriangleApplication {

private:
    GLFWwindow* window;
    VkInstance vulkan;

    void init() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

        uint32_t len = 0;
        const char * const * const tmp = glfwGetRequiredInstanceExtensions(&len);
        const std::vector<const char*> glfwExtensions(&tmp[0], &tmp[len] );
        const std::vector<const char*> validationLayers = {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation",
#endif
        };
        createVulkan(
            glfwExtensions,
            validationLayers,
            &vulkan
        );
        showVulkanExtensions();
        pickPhysicalDevice(vulkan);
    }


    void mainLoop() {
        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(vulkan, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }
};



int main()
{
    HelloTriangleApplication app;
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}