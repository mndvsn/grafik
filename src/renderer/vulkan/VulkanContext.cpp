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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void VulkanContext::Init(GLFWwindow* window)
{
    _window = window;

    CreateInstance();
    CreateSurface();
    
    _device = std::make_unique<VulkanDevice>(_instance, _surface, _validationLayers);

    CreatePipelineLayout();
    
    int width { 0 }, height { 0 };
    glfwGetFramebufferSize(_window, &width, &height);
    Resize(width, height); // will also create swap chain

    CreateModel();
    CreateCommandBuffers();
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

void VulkanContext::CreateSwapchain()
{
    if (_extent.width == 0 || _extent.height == 0) return;
    
    _device->GetDevice().waitIdle();

    if (!_swapChain)
    {
        _swapChain = std::make_unique<VulkanSwapChain>(*_device);
    }
    else
    {
        _swapChain = std::make_unique<VulkanSwapChain>(*_device, std::move(_swapChain));
        if (_swapChain->GetImageCount() != _commandBuffers.size())
        {
            FreeCommandBuffers();
            CreateCommandBuffers();
        }
    }
    
    //TODO: create unless any existing is compatible
    CreatePipeline();
    
    // reset flag
    _extentWasResized = false;
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
    assert(_swapChain && "Needed Swapchain not available");
    assert(_pipelineLayout && "Needed PipelineLayout not available");
    
    auto config = PipelineConfig { };
    config.renderPass = _swapChain->GetRenderPass();
    config.pipelineLayout = _pipelineLayout;

    _pipeline.reset(new VulkanPipeline {
        *_device,
        "data/shaders/vktest.vert.spv",
        "data/shaders/vktest.frag.spv",
        config });
}

void VulkanContext::CreateCommandBuffers()
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

}

void VulkanContext::FreeCommandBuffers()
{
    _device->GetDevice().freeCommandBuffers(_device->GetCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
    _commandBuffers.clear();
}

void VulkanContext::RecordCommandBuffer(int imageIndex) const
{
    const vk::CommandBuffer& buffer = _commandBuffers[imageIndex];
    
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

    const vk::RenderPassBeginInfo renderPassBeginInfo
    {
        .renderPass      = _swapChain->GetRenderPass(),
        .framebuffer     = _swapChain->GetFramebuffer(imageIndex),
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

    // transformation between pipeline output and target image
    const vk::Viewport viewport
    {
        .x              = 0.0f,
        .y              = 0.0f,
        .width          = static_cast<float>(_extent.width),
        .height         = static_cast<float>(_extent.height),
        .minDepth       = 0.0f,
        .maxDepth       = 1.0f
    };

    // crops image
    const vk::Rect2D scissor
    {
        .offset         = { 0, 0 },
        .extent         = _extent
    };
    buffer.setViewport(0, 1, &viewport);
    buffer.setScissor(0, 1, &scissor);

    // Bind command buffer to graphics binding point
    _pipeline->Bind(buffer);

    _model->Bind(buffer);
    _model->Draw(buffer);

    buffer.endRenderPass();
    buffer.end();
}

void VulkanContext::Resize(unsigned width, unsigned height)
{
    if (_extent.width == width && _extent.height == height) return;

    _extent = { width, height };
    _extentWasResized = true;

    // Crate new swap chain here to get rendering on resize
    CreateSwapchain();
}

void VulkanContext::SwapBuffers()
{
    if (_extent.width == 0 || _extent.height == 0) return;

    try
    {
        uint32_t imageIndex;
        
        // Wait for an image to become available to render to
        _swapChain->GetNextImage(imageIndex);
        
        RecordCommandBuffer(static_cast<int>(imageIndex));
        
        // Submit buffer to device graphics queue, and handle image swap based on present mode
        const auto result = _swapChain->SubmitCommandBuffers(&_commandBuffers[imageIndex], imageIndex);

        if (result == vk::Result::eSuboptimalKHR)
        {
            return CreateSwapchain();
        }
    }
    catch (vk::OutOfDateKHRError&)
    {
        // extent has changed, ie window resized/minimized
        return CreateSwapchain();
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to swap buffers in vulkan swap chain: " << err.what() << std::endl;
    }
}

VulkanContext::~VulkanContext()
{
    // wait for rendering to complete
    _device->GetDevice().waitIdle();
    VulkanContext::Resize(0, 0);

    // call destructors
    _model.reset();
    _device->GetDevice().destroyPipelineLayout(_pipelineLayout);
    _pipeline.reset();
    _swapChain.reset();
    _device.reset();

#ifdef _DEBUG
    _instance.destroyDebugUtilsMessengerEXT(_debugMessenger);
#endif
    
    _instance.destroySurfaceKHR(_surface);
    _instance.destroy();
}

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
