/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/GraphicsContext.h"
#include "renderer/vulkan/RenderDevice.h"


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

    void Shutdown() override;

#ifdef _DEBUG
    void InitDebug() const;
#endif

private:
    RenderDevice* _device { nullptr };
    std::unique_ptr<Pipeline> _pipeline { nullptr };
    std::unique_ptr<SwapChain> _swapChain { nullptr };
    VkPipelineLayout _pipelineLayout { nullptr };
    std::vector<VkCommandBuffer> _commandBuffers { };
};
