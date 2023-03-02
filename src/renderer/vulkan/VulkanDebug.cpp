/**
 * Grafik
 * Vulkan: Debug
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanDebug.h"


#ifdef _DEBUG
vk::Bool32 VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    (void)pUserData;
    
    std::cerr << "---------------\n";
    std::cerr << "Vulkan: ";

    std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity));
    std::cerr << " " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)) << " ";
    std::cerr << pCallbackData->pMessageIdName;
    std::cerr << " / ";
    std::cerr << pCallbackData->pMessage << std::endl;
    
    // __debugbreak();
    
    return false;
}

vk::DebugUtilsMessengerCreateInfoEXT GetDebugInfo()
{
    const auto severityFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
        /*| vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose*/;

    const auto typeFlags = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    const auto debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT
    {
        .messageSeverity    = severityFlags,
        .messageType        = typeFlags,
        .pfnUserCallback    = VulkanDebugCallback,
        .pUserData          = nullptr
    };
    return debugCreateInfo;
}

bool CheckDebugSupport(const std::vector<const char*>& requestedLayers)
{
    const auto availableLayers = vk::enumerateInstanceLayerProperties();
    for (const char* layerName : requestedLayers)
    {
        bool found { false };
        for (const auto& layer : availableLayers)
        {
            if (strcmp(layerName, layer.layerName) == 0)
            {
                found = true;
                break;
            }
        }
        // Not all layers were found so validation isn't supported
        if (!found) return false;
    }
    return true;
}
#endif