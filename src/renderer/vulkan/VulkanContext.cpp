/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanContext.h"

#include "renderer/vulkan/Pipeline.h"
#include "renderer/vulkan/SwapChain.h"


VulkanContext::VulkanContext()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanContext::Init(GLFWwindow* window)
{
    _window = window;

    int width { 0 }, height { 0 };
    glfwGetFramebufferSize(_window, &width, &height);
    
    _device = new RenderDevice { _window };
    const VkExtent2D extent { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    _swapChain = std::make_unique<SwapChain>(*_device, extent);

#   ifdef _DEBUG
    InitDebug();
#   endif

    CreatePipelineLayout();
    CreatePipeline();
    CreateCommandBuffer();
}

void VulkanContext::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo { };
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(_device->device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error { "Failed to create Vulkan Pipeline Layout!" };
    }
}

void VulkanContext::CreatePipeline()
{
    auto config = PipelineConfig { _swapChain->width(), _swapChain->height() };
    config.renderPass = _swapChain->getRenderPass();
    config.pipelineLayout = _pipelineLayout;

    _pipeline = std::make_unique<Pipeline>(
        *_device, 
        "data/shaders/vktest.vert.spv",
        "data/shaders/vktest.frag.spv",
        config);
}

void VulkanContext::CreateCommandBuffer()
{
    // Set number of command buffers to swapchain images/fb count
    _commandBuffers.resize(_swapChain->imageCount());

    VkCommandBufferAllocateInfo allocateInfo { };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = _device->getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

    if (vkAllocateCommandBuffers(_device->device(), &allocateInfo, _commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error { "Failed to allocate vulkan command buffers!" };
    }

    for (int i = 0; const VkCommandBuffer& buffer : _commandBuffers)
    {
        VkCommandBufferBeginInfo beginInfo { };
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        const auto beginResult = vkBeginCommandBuffer(buffer, &beginInfo);
        if (beginResult != VK_SUCCESS)
        {
            throw std::runtime_error { "Failed to begin recording command buffer!" };
        }

        std::array<VkClearValue, 2> clearValues { };
        clearValues[0].color = {{ 0.6f, 0.6f, 0.6f, 1.0f }};
        clearValues[1].depthStencil = { 1.0f, 0 };
        
        VkRenderPassBeginInfo renderPassBeginInfo { };
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = _swapChain->getRenderPass();
        renderPassBeginInfo.framebuffer = _swapChain->getFrameBuffer(i);

        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = _swapChain->getSwapChainExtent();
        
        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassBeginInfo.pClearValues = clearValues.data();

        /* VK_SUBPASS_CONTENTS
         * INLINE = subsequent render pass commands will be embedded in the primary command buffer
         * SECONDARY_COMMAND_BUFFERS = subsequent commands will be embedded in secondary command buffers */
        vkCmdBeginRenderPass(buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Bind command buffer to graphics binding point
        _pipeline->Bind(buffer);
        vkCmdDraw(buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(buffer);
        const auto endResult = vkEndCommandBuffer(buffer);
        if (endResult != VK_SUCCESS)
        {
            throw std::runtime_error { "Failed to end recording command buffer!" };
        }
        
        i++;
    }
}

void VulkanContext::SwapBuffers()
{
    //TODO: SwapChain might be outdated if extent is changed, ie window resized/minimized
    uint32_t imageIndex { 0 };
    
    const auto queryResult = _swapChain->acquireNextImage(&imageIndex);
    if (queryResult != VK_SUCCESS && queryResult != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error { "Failed to query next swapchain image!" };
    }

    // Submit buffer to device graphics queue, and handle image swap based on present mode
    const auto submitResult = _swapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
    if (submitResult != VK_SUCCESS)
    {
        throw std::runtime_error { "Failed to present image in swapchain!" };
    }
}

void VulkanContext::Shutdown()
{
    // wait for rendering to complete
    vkDeviceWaitIdle(_device->device());
}

VulkanContext::~VulkanContext()
{
    vkDestroyPipelineLayout(_device->device(), _pipelineLayout, nullptr);
    delete _device;
}

#ifdef _DEBUG
void VulkanContext::InitDebug() const
{
    // Print adapter info
    std::cout << "Vulkan" << "\n";
}
#endif
