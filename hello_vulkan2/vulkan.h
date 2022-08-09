#pragma once
#include <vulkan/vulkan.h>

#include <vector>

void showVulkanExtensions();
bool validateVulkanLayers(const std::vector<const char*>& validationLayers);
bool isDeviceSuitable(const VkPhysicalDevice& device);
void pickPhysicalDevice(const VkInstance& vulkan);
void createVulkan(const std::vector<const char*>& glfwExtensions,
                  const std::vector<const char*>& validationLayers,
                  VkInstance* const vulkan);
