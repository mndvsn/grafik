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


class VulkanModel;
class VulkanDevice;
class VulkanSwapChain;
class VulkanPipeline;

struct SimplePushConstantData
{
    glm::mat4 transform { 1.0f };
    alignas(16) glm::vec3 color { };
};

class VulkanContext : public GraphicsContext
{
public:
    VulkanContext();
    ~VulkanContext() override;

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    void Init(GLFWwindow* window) override;
    void SwapBuffers() override;
    
    void Resize(unsigned width, unsigned height) override;
    [[nodiscard]] std::pair<unsigned, unsigned> GetSize() const override { return { _extent.width, _extent.height }; }

private:
    void CreateInstance();
    void CreateSurface();

    void CreateSwapchain();
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateModel();
    void CreateCommandBuffers();
    void FreeCommandBuffers();
    void RecordCommandBuffer(int imageIndex) const;
    
    static std::vector<const char*> GetRequiredExtensions();

#ifdef _DEBUG
    void InitDebug();
#endif

    vk::Instance _instance { };
    vk::SurfaceKHR _surface { };
    
    std::unique_ptr<VulkanDevice> _device { };
    std::unique_ptr<VulkanPipeline> _pipeline { };
    std::unique_ptr<VulkanSwapChain> _swapChain { };
    std::vector<vk::CommandBuffer> _commandBuffers { };
    vk::PipelineLayout _pipelineLayout { };
    
    vk::Extent2D _extent { 0, 0 };
    bool _extentWasResized { false };

    std::unique_ptr<VulkanModel> _model { };
    
    vk::DebugUtilsMessengerEXT _debugMessenger { };
    std::vector<const char*> _validationLayers { "VK_LAYER_KHRONOS_validation" };
};
