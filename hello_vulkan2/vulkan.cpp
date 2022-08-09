#include "vulkan.h"

#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void showVulkanExtensions() {
  uint32_t extensionCount = 0;
#pragma warning( \
    suppress : 26812)  // https://docs.microsoft.com/en-us/cpp/preprocessor/warning
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());
  std::cout << extensionCount << " Vulkan Extensions available:\n";

  for (const auto &extension : extensions) {
    std::cout << '\t' << extension.extensionName << " v"
              << extension.specVersion << '\n';
  }
}

bool validateVulkanLayers(const std::vector<const char *> &validationLayers) {
  uint32_t layerCount = 0;

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  std::cout << layerCount << " Vulkan Layers available:\n";
  for (const auto &layerProperties : availableLayers) {
    std::cout << std::left << '\t' << std::setw(40) << layerProperties.layerName
              << " " << layerProperties.description << '\n';
  }

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
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

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;

  bool isComplete() { return graphicsFamily.has_value(); }
};

template <class T>
struct show_type;

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::cout << queueFamilyCount << " Vulkan Queue Families available:\n";

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    std::cout << '\t' << queueFamily.queueCount << "x "
              << queueFamily.queueFlags << '\n';
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    i++;
  }
  return indices;
}

bool isDeviceSuitable(const VkPhysicalDevice &device) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  QueueFamilyIndices indices = findQueueFamilies(device);

  return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
         deviceFeatures.geometryShader && indices.isComplete();
}

void pickPhysicalDevice(const VkInstance &vulkan) {
  VkPhysicalDevice chosenDevice = VK_NULL_HANDLE;
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(vulkan, &deviceCount, nullptr);
  std::cout << deviceCount << " Vulkan Devices available:\n";
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(vulkan, &deviceCount, devices.data());

  for (const auto &device : devices) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

    std::cout << std::left << '\t' << std::setw(40) << device << " "
              << physicalDeviceProperties.deviceName << '\n';
  }

  for (const auto &device : devices) {
    if (isDeviceSuitable(device)) {
      chosenDevice = device;
      break;
    }
  }

  if (chosenDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

void createVulkan(const std::vector<const char *> &glfwExtensions,
                  const std::vector<const char *> &validationLayers,
                  VkInstance *const vulkan) {
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

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(glfwExtensions.size());
  createInfo.ppEnabledExtensionNames = glfwExtensions.data();

  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();

#pragma warning( \
    suppress : 26812)  // https://docs.microsoft.com/en-us/cpp/preprocessor/warning
  if (vkCreateInstance(&createInfo, nullptr, vulkan) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}
