/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/GraphicsContext.h"

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


class VulkanUI;
class VulkanDevice;
class VulkanSwapChain;

class VulkanContext : public GraphicsContext
{
public:
    VulkanContext();
    ~VulkanContext() override;

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;
    VulkanContext(const VulkanContext&&) = delete;
    VulkanContext& operator=(const VulkanContext&&) = delete;

    void Init(GLFWwindow* window) override;

    vk::CommandBuffer BeginFrame();
    void BeginRenderPass(vk::CommandBuffer buffer);
    void EndRenderPass() const;
    void SwapBuffers() override;
    
    void Resize(unsigned width, unsigned height) override;
    [[nodiscard]] std::pair<unsigned, unsigned> GetSize() const override { return { _extent.width, _extent.height }; }

    void SetClearColor(const glm::vec4& color) { _clearColor = color; }
    
    [[nodiscard]] vk::Instance& GetInstance() { return _instance; }
    [[nodiscard]] VulkanDevice* GetDevice() const { return _device.get(); }
    [[nodiscard]] VulkanSwapChain* GetSwapChain() const { return _swapChain.get(); }
    [[nodiscard]] vk::CommandBuffer GetCommandBuffer() const
    {
        GK_ASSERT(FrameInProgress(), "No frame in progress")
        return _commandBuffers.at(_currentFrame);
    }

    [[nodiscard]] bool FrameInProgress() const { return _inFrameRender; }
    [[nodiscard]] uint32_t GetCurrentFrame() const { return _currentFrame; }

    void AttachUI(const std::weak_ptr<VulkanUI>& ui) { _vulkanUI = ui; }
    void DetachUI() { _vulkanUI.reset(); }
    
private:
    void CreateInstance();
    void CreateSurface();

    void CreateSwapchain();
    void CreateCommandBuffers();
    
    static std::vector<const char*> GetRequiredExtensions();

#ifdef GK_DEBUG
    void InitDebug();
#endif

    vk::Instance _instance { };
    vk::SurfaceKHR _surface { };
    
    std::unique_ptr<VulkanDevice> _device { };
    std::unique_ptr<VulkanSwapChain> _swapChain { };
    std::vector<vk::CommandBuffer> _commandBuffers { };
    std::weak_ptr<VulkanUI> _vulkanUI { };
    
    uint32_t _currentImage { 0 };
    uint32_t _currentFrame { 0 };
    vk::Extent2D _extent { 0, 0 };
    bool _inFrameRender { false };
    bool _extentWasResized { false };

    glm::vec4 _clearColor { 0.0f, 0.0f, 0.0f, 1.0f };

    std::vector<const char*> _validationLayers { "VK_LAYER_KHRONOS_validation" };
};
