/**
 * Grafik
 * Vulkan: Device
 * Copyright Martin Furuberg
 */
#pragma once
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
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
    vk::DescriptorPool  _descriptorPool { };
    vk::Queue           _graphicsQueue { };
    vk::Queue           _presentQueue { };
    
public:
    VulkanDevice(vk::Instance& instance, vk::SurfaceKHR& surface, std::vector<const char*>& validationLayers);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;

    [[nodiscard]] vk::Instance& GetInstance() const { return _instance; }
    [[nodiscard]] vk::Device& GetDevice() { return _device; }
    [[nodiscard]] vk::PhysicalDevice& GetPhysicalDevice() { return _physicalDevice; }
    [[nodiscard]] vk::SurfaceKHR& GetSurface() const { return _surface; }
    [[nodiscard]] vk::CommandPool& GetCommandPool() { return _commandPool; }
    [[nodiscard]] vk::DescriptorPool& GetDescriptorPool() { return _descriptorPool; }

    [[nodiscard]] vk::Queue GetGraphicsQueue() const { return _graphicsQueue; }
    [[nodiscard]] vk::Queue GetPresentQueue() const { return _presentQueue; }

    [[nodiscard]] SwapChainSupportDetails GetSwapChainSupport() const { return CheckSwapChainSupport(_physicalDevice); }
    [[nodiscard]] QueueFamilyIndices FindPhysicalQueueFamilies() const { return FindQueueFamilies(_physicalDevice); }
    [[nodiscard]] vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlagBits features) const;
    [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties) const;

    void CreateImage(const vk::ImageCreateInfo& imageInfo, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& memory) const;
    [[nodiscard]] vk::DescriptorPool CreateDescriptorPool(const std::vector<vk::DescriptorPoolSize>& poolSizes, unsigned maxSets) const;

    // Buffer helper functions
    void CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) const;
    [[nodiscard]] vk::CommandBuffer BeginSingleTimeCommands() const;
    void EndSingleTimeCommands(vk::CommandBuffer commandBuffer) const;
    void CopyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size);
    void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height, uint32_t layerCount);

private:
    void SelectPhysicalDevice();
    void CreateLogicalDevice();
    void InitCommandPool();
    void InitDescriptorPool();

    [[nodiscard]] bool CheckDevice(const vk::PhysicalDevice& device) const;
    [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device) const;
    [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const;
    [[nodiscard]] SwapChainSupportDetails CheckSwapChainSupport(const vk::PhysicalDevice& device) const;

    const std::vector<const char*>&     _validationLayers;
    const std::vector<const char*>      _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};
