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


class VulkanDevice;
class VulkanSwapChain;
class VulkanPipeline;

static std::string ApplicationName  { "Grafik" };
static std::string EngineName       { "Vulkan" };

class VulkanContext : public GraphicsContext
{
public:
    VulkanContext();
    ~VulkanContext() override;

    void Init(GLFWwindow* window) override;
    void SwapBuffers() override;
    
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffer();

    void Resize(unsigned width, unsigned height) override;
    [[nodiscard]] std::pair<unsigned, unsigned> GetSize() const override { return { _extent.width, _extent.height }; }

    void Shutdown() override;

private:
    void CreateInstance();
    void CreateSurface();

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
    
    vk::DebugUtilsMessengerEXT _debugMessenger { };
    std::vector<const char*> _validationLayers { "VK_LAYER_KHRONOS_validation" };
};
