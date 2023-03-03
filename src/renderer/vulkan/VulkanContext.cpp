/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanContext.h"

#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanSwapChain.h"
#include "renderer/vulkan/VulkanPipeline.h"
#include "renderer/vulkan/VulkanModel.h"
#include "renderer/vulkan/VulkanDebug.h"

#include <GLFW/glfw3.h>


VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanContext::VulkanContext()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanContext::Init(GLFWwindow* window)
{
    _window = window;

    CreateInstance();
    CreateSurface();
    
    int width { 0 }, height { 0 };
    glfwGetFramebufferSize(_window, &width, &height);
    Resize(width, height);

    _device = std::make_unique<VulkanDevice>(_instance, _surface, _validationLayers);
    _swapChain = std::make_unique<VulkanSwapChain>(*_device);

    CreatePipelineLayout();
    CreatePipeline();
    CreateModel();
    CreateCommandBuffer();
}

void VulkanContext::CreateInstance()
{
    std::cout << "Vulkan" << std::endl;

#ifdef _DEBUG
    if (!CheckDebugSupport(_validationLayers))
    {
        throw std::runtime_error { "Validation layers are not supported!" };
    }
    const auto debugCreateInfo = GetDebugInfo();
#endif

    const auto appInfo = vk::ApplicationInfo
    {
        .pApplicationName       = ApplicationName.c_str(),
        .applicationVersion     = 1,
        .pEngineName            = EngineName.c_str(),
        .engineVersion          = 1,
        .apiVersion             = VK_API_VERSION_1_1
    };

    const auto extensions = GetRequiredExtensions();

    const vk::InstanceCreateInfo createInfo
    {
        #ifdef _DEBUG
        .pNext                   = &debugCreateInfo,
        #endif
        .pApplicationInfo        = &appInfo,
        #ifdef _DEBUG
        // Add validation layers to check for errors
        .enabledLayerCount       = static_cast<uint32_t>(_validationLayers.size()),
        .ppEnabledLayerNames     = _validationLayers.data(),
        #endif
        .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    // Dynamically load extensions
    const vk::DynamicLoader dl { };
    const auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
    
    try
    {
        _instance = vk::createInstance(createInfo, nullptr);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(_instance);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan instance: " << error.what() << std::endl;
    }
}

void VulkanContext::CreateSurface()
{
    VkSurfaceKHR surface;
    const auto result = glfwCreateWindowSurface(_instance, _window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
    _surface = surface;
}

void VulkanContext::CreateModel()
{
    std::vector<VulkanModel::Vertex> vertices
    {
        { .position = {  0.0f, -0.5f, 0.0f }, .color = { 1.0f, 0.0f, 0.0f }},
        { .position = {  0.5f,  0.5f, 0.0f }, .color = { 0.0f, 1.0f, 0.0f }},
        { .position = { -0.5f,  0.5f, 0.0f }, .color = { 0.0f, 0.0f, 1.0f }}
    };
    
    _model = std::make_unique<VulkanModel>(*_device, vertices);
}

void VulkanContext::CreatePipelineLayout()
{
    const vk::PipelineLayoutCreateInfo pipelineLayoutInfo
    {
        .setLayoutCount             = 0,
        .pushConstantRangeCount     = 0,
    };
    
    try
    {
        _pipelineLayout = _device->GetDevice().createPipelineLayout(pipelineLayoutInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan Pipeline Layout: " << error.what() << std::endl;
    }
}

void VulkanContext::CreatePipeline()
{
    auto config = PipelineConfig { _extent.width, _extent.height };
    config.renderPass = _swapChain->GetRenderPass();
    config.pipelineLayout = _pipelineLayout;

    _pipeline = std::make_unique<VulkanPipeline>(
        *_device,
        "data/shaders/vktest.vert.spv",
        "data/shaders/vktest.frag.spv",
        config);
}

void VulkanContext::CreateCommandBuffer()
{
    // Set number of command buffers to swapchain images/fb count
    _commandBuffers.resize(_swapChain->GetImageCount());

    const vk::CommandBufferAllocateInfo allocInfo
    {
        .commandPool = _device->GetCommandPool(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = static_cast<uint32_t>(_commandBuffers.size())
    };

    if (_device->GetDevice().allocateCommandBuffers(&allocInfo, _commandBuffers.data()) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to allocate vulkan command buffers!" };
    }

    for (int i = 0; const vk::CommandBuffer& buffer : _commandBuffers)
    {
        constexpr auto beginInfo = vk::CommandBufferBeginInfo
        {
            .flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse
        };
        
        if (buffer.begin(&beginInfo) != vk::Result::eSuccess)
        {
            throw std::runtime_error { "Failed to begin recording command buffer!" };
        }

        const vk::ClearValue color
        {
            .color           = { std::array { 0.6f, 0.6f, 0.6f, 1.0f } }
        };
        constexpr vk::ClearValue depthStencil
        {
            .depthStencil    = { .depth = 1.0f, .stencil = 0 }
        };
        const std::vector clearValues { color, depthStencil };

        vk::RenderPassBeginInfo renderPassBeginInfo
        {
            .renderPass      = _swapChain->GetRenderPass(),
            .framebuffer     = _swapChain->GetFramebuffer(i),
            .renderArea      = {
                .offset      = { 0, 0 },
                .extent      = _swapChain->GetExtent()
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues    = clearValues.data()
        };

        /* VK_SUBPASS_CONTENTS
         * INLINE = subsequent render pass commands will be embedded in the primary command buffer
         * SECONDARY_COMMAND_BUFFERS = subsequent commands will be embedded in secondary command buffers */
        buffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        // Bind command buffer to graphics binding point
        _pipeline->Bind(buffer);

        _model->Bind(buffer);
        _model->Draw(buffer);

        buffer.endRenderPass();
        buffer.end();

        i++;
    }
}

void VulkanContext::Resize(unsigned width, unsigned height)
{
    _extent.width = width;
    _extent.height = height;

    //TODO: invalidate current swapchain
}

void VulkanContext::SwapBuffers()
{
    //TODO: SwapChain might be outdated if extent is changed, ie window resized/minimized
    if (_extent.width == 0 || _extent.height == 0) return;

    try
    {
        uint32_t imageIndex;
        // Wait for an image to become available to render to
        if (!_swapChain->GetNextImage(imageIndex)) return;
        
        // Submit buffer to device graphics queue, and handle image swap based on present mode
        _swapChain->SubmitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to swap buffers in vulkan swap chain: " << err.what() << std::endl;
    }
}

void VulkanContext::Shutdown()
{
    // wait for rendering to complete
    _device->GetDevice().waitIdle();

    _model.reset();
    
    _device->GetDevice().destroyPipelineLayout(_pipelineLayout);
    _pipeline->Destroy();
    
    _swapChain->Shutdown();
    _device->Shutdown();

#ifdef _DEBUG
    _instance.destroyDebugUtilsMessengerEXT(_debugMessenger);
#endif
    
    _instance.destroySurfaceKHR(_surface);
    _instance.destroy();
    Resize(0, 0);
}

VulkanContext::~VulkanContext() = default;

std::vector<const char*> VulkanContext::GetRequiredExtensions()
{
    uint32_t glfwExtensionsCount { 0 };
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    if (glfwExtensionsCount == 0)
    {
        throw std::runtime_error { "GLFW required extensions returned an error" };
    }

    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

    #ifdef _DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif
    
    return extensions;
}

#ifdef _DEBUG
void VulkanContext::InitDebug()
{
    const auto debugCreateInfo = GetDebugInfo();
    try
    {
        _debugMessenger = _instance.createDebugUtilsMessengerEXT(debugCreateInfo);
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to create Vulkan debug messenger: " << err.what() << std::endl;
    }
}
#endif
