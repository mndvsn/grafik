/**
 * Grafik
 * Vulkan: Swap chain
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/vulkan/VulkanDevice.h"


class VulkanDevice;

class VulkanSwapChain
{
    static constexpr int MAX_FRAMES_IN_FLIGHT { 2 };

public:
    VulkanSwapChain(VulkanDevice& device);
    ~VulkanSwapChain() = default;

    VulkanSwapChain(const VulkanSwapChain&) = delete;
    VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
    VulkanSwapChain(VulkanSwapChain&&) = delete;
    VulkanSwapChain& operator=(VulkanSwapChain&&) = delete;

    [[nodiscard]] vk::RenderPass GetRenderPass() const { return _renderPass; }
    [[nodiscard]] vk::Framebuffer GetFramebuffer(int index) const { return _framebuffers[index]; }
    [[nodiscard]] vk::ImageView GetImageView(int index) const { return _imageViews[index]; }
    
    [[nodiscard]] vk::Extent2D GetExtent() const { return _extent; }
    [[nodiscard]] vk::Format GetImageFormat() const { return _imageFormat; }
    [[nodiscard]] size_t GetImageCount() const { return _images.size(); }

    bool GetNextImage(uint32_t& imageIndex) const;
    void SubmitCommandBuffers(const vk::CommandBuffer* buffers, const uint32_t* imageIndex);
    
    void Shutdown();

private:
    void Init();
    void CreateImageViews();
    void CreateDepthResources();
    void CreateRenderPass();
    void CreateFramebuffers();
    void CreateSyncObjects();

    [[nodiscard]] vk::SurfaceFormatKHR SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;
    [[nodiscard]] vk::PresentModeKHR SelectPresentMode(const std::vector<vk::PresentModeKHR>& availableModes) const;
    [[nodiscard]] vk::Extent2D SelectImageExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

    [[nodiscard]] static vk::Extent2D GetWindowExtent();
    [[nodiscard]] vk::Format FindDepthFormat() const;

    [[nodiscard]] float GetAspectRatio() const
    {
        return static_cast<float>(_extent.width) / static_cast<float>(_extent.height);
    }
    
private:
    VulkanDevice&                   _device;
    vk::SwapchainKHR                _swapChain { };
    vk::RenderPass                  _renderPass { };

    vk::Format                      _imageFormat { vk::Format::eUndefined };
    vk::Extent2D                    _extent { };

    std::vector<vk::Framebuffer>    _framebuffers { };
    std::vector<vk::Image>          _images { };
    std::vector<vk::ImageView>      _imageViews { };
    std::vector<vk::Image>          _depthImages { };
    std::vector<vk::ImageView>      _depthImageViews { };
    std::vector<vk::DeviceMemory>   _depthImageMemory { };

    std::vector<vk::Semaphore>      _imageAvailableSemaphores { };
    std::vector<vk::Semaphore>      _renderFinishedSemaphores { };
    std::vector<vk::Fence>          _inFlightFences { };
    std::vector<vk::Fence>          _imagesInFlight { };
    size_t _currentFrame            { 0 };
};
