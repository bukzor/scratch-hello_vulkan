#include <vulkan/vulkan.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

#include "vulkan.h"

void showVulkanExtensions() {
    uint32_t extensionCount = 0;
    #pragma warning( suppress : 26812 )  // https://docs.microsoft.com/en-us/cpp/preprocessor/warning
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << extensionCount << " Vulkan Extensions available:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << " v" << extension.specVersion << '\n';
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

bool isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader;
}

void pickPhysicalDevice(VkInstance &vulkan) {
    VkPhysicalDevice chosenDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkan, &deviceCount, nullptr);
    std::cout << deviceCount << " Vulkan Devices available:\n";
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkan, &deviceCount, devices.data());


    for (const auto& device : devices) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(
            device,
            &physicalDeviceProperties
        );

        std::cout << std::left << '\t'
            << std::setw(40) << device << " "
            << physicalDeviceProperties.deviceName << '\n'
            ;
    }

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            chosenDevice = device;
            break;
        }
    }

    if (chosenDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void createVulkan(const std::vector<const char*> &glfwExtensions, const std::vector<const char*> &validationLayers, VkInstance *vulkan) {
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


    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    #pragma warning( suppress : 26812 )  // https://docs.microsoft.com/en-us/cpp/preprocessor/warning
    if (vkCreateInstance(&createInfo, nullptr, vulkan) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}