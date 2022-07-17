#pragma once
void showVulkanExtensions();
bool validateVulkanLayers(const std::vector<const char*> validationLayers);
void pickPhysicalDevice(VkInstance& vulkan);
void createVulkan(const std::vector<const char*>& glfwExtensions, const std::vector<const char*>& validationLayers, VkInstance* vulkan);

