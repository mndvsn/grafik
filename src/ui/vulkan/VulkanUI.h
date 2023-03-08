/**
* Grafik
 * Vulkan UI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "ui/UI.h"
#include "renderer/vulkan/VulkanContext.h"


class VulkanUI : public UI
{
public:
    ~VulkanUI() override;

    void Init(GLFWwindow* window) override;

    void Begin() override;
    void End() override;
    
    vk::CommandBuffer& Render(uint32_t imageIndex);

    void CreateContext();
    
private:
    void CreateFontTexture();
    void CreateRenderPass();
    void CreateDescriptorPool();
    void CreateCommandBuffers(uint32_t count);
    void DestructVulkanObjects() const;

    std::shared_ptr<VulkanContext> _context { };
    vk::Instance _instance { };
    VulkanDevice* _vulkanDevice { };
    VulkanSwapChain* _vulkanSwapchain { };

    vk::RenderPass _renderPass { };
    vk::DescriptorPool _descriptorPool { };
    std::vector<vk::Framebuffer> _framebuffers { };
    std::vector<vk::CommandBuffer> _commandBuffers { };
};
