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

using namespace std::string_literals;

const std::vector<const char*> validationLayers = {
#ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation",
#endif
};

class HelloTriangleApplication {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance vulkan;


    void init() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

        createVulkan();
        showVulkanExtensions();
    }
    void createVulkan() {
        if (!validateVulkanLayers(validationLayers)) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        #pragma warning( suppress : 26812 )  // https://docs.microsoft.com/en-us/cpp/preprocessor/warning
        VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkan);

        if (vkCreateInstance(&createInfo, nullptr, &vulkan) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }
    bool validateVulkanLayers(const std::vector<const char*> validationLayers) {
        uint32_t layerCount;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::cout << layerCount << " Vulkan Layers available:\n";
        for (const auto& layerProperties : availableLayers) {
            std::cout << std::left << '\t'
                << std::setw(40) << layerProperties.layerName << " "
                << layerProperties.description << '\n'
                ;
        }

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
    void showVulkanExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::cout << extensionCount << " extensions available:\n";

        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << " v" << extension.specVersion << '\n';
        }
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