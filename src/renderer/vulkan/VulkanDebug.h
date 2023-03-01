/**
 * Grafik
 * Vulkan: Debug
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include <vulkan/vulkan.hpp>

static VKAPI_ATTR vk::Bool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

vk::DebugUtilsMessengerCreateInfoEXT GetDebugInfo();
[[nodiscard]] bool CheckDebugSupport(const std::vector<const char*>& requestedLayers);
