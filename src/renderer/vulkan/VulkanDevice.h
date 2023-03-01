/**
 * Grafik
 * Vulkan: Device
 * Copyright Martin Furuberg
 */
#pragma once

#include <vulkan/vulkan.hpp>


struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily { };
    std::optional<uint32_t> presentFamily { };

    [[nodiscard]] bool IsComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR          capabilities { };
    std::vector<vk::SurfaceFormatKHR>   formats { };
    std::vector<vk::PresentModeKHR>     presentModes { };
};

class VulkanDevice
{
    vk::Instance&       _instance;
    vk::SurfaceKHR&     _surface;
    vk::PhysicalDevice  _physicalDevice { };
    vk::Device          _device { };
    vk::CommandPool     _commandPool { };
    vk::Queue           _graphicsQueue { };
    vk::Queue           _presentQueue { };
    
public:
    VulkanDevice(vk::Instance& instance, vk::SurfaceKHR& surface, std::vector<const char*>& validationLayers);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice&) = delete;
    void operator=(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;

    vk::Device& GetDevice() { return _device; }
    vk::CommandPool& GetCommandPool() { return _commandPool; }

    void Shutdown() const;
    
private:
    void SelectPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();

    [[nodiscard]] bool CheckDevice(const vk::PhysicalDevice& device) const;
    [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device) const;
    [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const;
    [[nodiscard]] SwapChainSupportDetails CheckSwapChainSupport(const vk::PhysicalDevice& device) const;

    const std::vector<const char*>&     _validationLayers;
    const std::vector<const char*>      _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};
