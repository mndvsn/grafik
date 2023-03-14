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
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT { 2 };

    VulkanSwapChain(VulkanDevice& device);
    VulkanSwapChain(VulkanDevice& device, std::unique_ptr<VulkanSwapChain> previous);
    ~VulkanSwapChain();

    VulkanSwapChain(const VulkanSwapChain&) = delete;
    VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

    [[nodiscard]] vk::SwapchainKHR GetSwapChain() const { return _swapChain; }
    [[nodiscard]] vk::RenderPass GetRenderPass() const { return _renderPass; }
    [[nodiscard]] vk::Framebuffer GetFramebuffer(int index) const { return _framebuffers[index]; }
    [[nodiscard]] vk::ImageView GetImageView(int index) const { return _imageViews[index]; }
    
    [[nodiscard]] vk::Extent2D GetExtent() const { return _extent; }
    [[nodiscard]] vk::Format GetImageFormat() const { return _imageFormat; }
    [[nodiscard]] vk::Format GetDepthFormat() const { return _depthFormat; }
    [[nodiscard]] size_t GetImageCount() const { return _images.size(); }

    [[nodiscard]] bool CompareImageFormats(const VulkanSwapChain& otherSwap) const
    {
        return _imageFormat == otherSwap.GetImageFormat() && _depthFormat == otherSwap.GetDepthFormat();
    }
    
    void GetNextImage(uint32_t& imageIndex) const;
    vk::Result SubmitCommandBuffers(const std::vector<vk::CommandBuffer>& buffers, uint32_t imageIndex);

    [[nodiscard]] vk::Framebuffer CreateFramebuffer(const vk::RenderPass& renderPass, const std::vector<vk::ImageView>& attachments) const;
    [[nodiscard]] vk::RenderPass CreateRenderPass(const vk::SubpassDescription& subpass, const vk::SubpassDependency& dependency, const std::vector<vk::AttachmentDescription>& attachments) const;
    
private:
    void Init();
    void InitSwapChain();
    void InitImageViews();
    void InitDepthResources();
    void InitRenderPass();
    void InitFramebuffers();
    void InitSyncObjects();

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
    VulkanDevice&                     _device;
    vk::SwapchainKHR                  _swapChain { };
    vk::RenderPass                    _renderPass { };

    vk::Format                        _imageFormat { vk::Format::eUndefined };
    vk::Format                        _depthFormat { vk::Format::eUndefined };
    vk::Extent2D                      _extent { };

    std::vector<vk::Framebuffer>      _framebuffers { };
    std::vector<vk::Image>            _images { };
    std::vector<vk::ImageView>        _imageViews { };
    std::vector<vk::Image>            _depthImages { };
    std::vector<vk::ImageView>        _depthImageViews { };
    std::vector<vk::DeviceMemory>     _depthImageMemory { };

    std::unique_ptr<VulkanSwapChain>  _reusableSwapChain { };

    std::vector<vk::Semaphore>        _imageAvailableSemaphores { };
    std::vector<vk::Semaphore>        _renderFinishedSemaphores { };
    std::vector<vk::Fence>            _inFlightFences { };
    std::vector<vk::Fence>            _imagesInFlight { };
    size_t _currentFrame              { 0 };
};
