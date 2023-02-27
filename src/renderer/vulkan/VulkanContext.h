/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/GraphicsContext.h"
#include "renderer/vulkan/RenderDevice.h"

// #include <vulkan/vulkan.hpp>


class Pipeline;
class SwapChain;

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

#ifdef _DEBUG
    void InitDebug() const;
#endif

private:
    std::unique_ptr<RenderDevice> _device { nullptr };
    std::unique_ptr<Pipeline> _pipeline { nullptr };
    std::unique_ptr<SwapChain> _swapChain { nullptr };
    std::vector<VkCommandBuffer> _commandBuffers { };
    VkPipelineLayout _pipelineLayout { nullptr };
    VkExtent2D _extent { 0, 0 };
};
