/**
 * Grafik
 * Vulkan: Context
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanContext.h"

#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanSwapChain.h"
#include "renderer/vulkan/VulkanDebug.h"
#include "ui/vulkan/VulkanUI.h"

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

#ifdef GK_DEBUG
    InitDebug();
#endif
    
    int width { 0 }, height { 0 };
    glfwGetFramebufferSize(_window, &width, &height);
    Resize(width, height); // will also create swap chain

    CreateCommandBuffers();
}

void VulkanContext::CreateInstance()
{
#ifdef GK_DEBUG
    Log::Info("{:*^50}", " Vulkan ");
    if (!CheckDebugSupport(_validationLayers))
    {
        throw std::runtime_error { "Validation layers are not supported!" };
    }
    const auto debugCreateInfo = GetDebugInfo();
#endif

    const auto appInfo = vk::ApplicationInfo
    {
        .pApplicationName       = Grafik::ApplicationName,
        .applicationVersion     = 1,
        .pEngineName            = Grafik::EngineName,
        .engineVersion          = 1,
        .apiVersion             = VK_MAKE_API_VERSION(0, Grafik::VulkanAPIMajor, Grafik::VulkanAPIMinor,
                                  Grafik::VulkanAPIPatch)
    };

    const auto extensions = GetRequiredExtensions();

    const vk::InstanceCreateInfo createInfo
    {
        #ifdef GK_DEBUG
        .pNext                   = &debugCreateInfo,
        #endif
        .pApplicationInfo        = &appInfo,
        #ifdef GK_DEBUG
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

    try
    {
        if (!_swapChain)
        {
            // Create new
            _swapChain = std::make_unique<VulkanSwapChain>(*_device);
        }
        else
        {
            // Recreate/reuse
            _swapChain = std::make_unique<VulkanSwapChain>(*_device, std::move(_swapChain));
        }
    }
    catch (std::exception& err)
    {
        Log::Error("Error: {}", err.what());
    }

    // Update ImGUI with new swapchain
    if (!_vulkanUI.expired())
    {
        _vulkanUI.lock()->CreateContext();
    }
    
    // reset flag
    _extentWasResized = false;
}

void VulkanContext::CreateCommandBuffers()
{
    // Set number of command buffers to swapchain images in flight
    _commandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

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

vk::CommandBuffer VulkanContext::BeginFrame()
{
    GK_ASSERT(!FrameInProgress(), "Frame is already in progress!")
    if (_extent.width == 0 || _extent.height == 0) return nullptr;
    
    try
    {
        // Wait for an image to become available to render to
        _swapChain->GetNextImage(_currentImage);
        return _commandBuffers.at(_currentFrame);
    }
    catch (vk::OutOfDateKHRError&)
    {
        // extent has changed, ie window resized/minimized
        CreateSwapchain();
    }
    catch (vk::SystemError& err)
    {
        Log::Error("Failed to begin new frame: {}", err.what());
    }
    return nullptr;
}

void VulkanContext::BeginRenderPass(vk::CommandBuffer buffer)
{
    _inFrameRender = true;
    GK_ASSERT(buffer == GetCommandBuffer(), "Command buffer mismatch")

    constexpr auto beginInfo = vk::CommandBufferBeginInfo
    {
        .flags           = vk::CommandBufferUsageFlagBits::eSimultaneousUse
    };

    if (buffer.begin(&beginInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to begin recording command buffer!" };
    }

    const vk::ClearValue color
    {
        .color           = { std::array { 0.08f, 0.08f, 0.08f, 1.0f } }
    };
    constexpr vk::ClearValue depthStencil
    {
        .depthStencil    = { .depth = 1.0f, .stencil = 0 }
    };
    const std::vector clearValues { color, depthStencil };

    const vk::RenderPassBeginInfo renderPassBeginInfo
    {
        .renderPass      = _swapChain->GetRenderPass(),
        .framebuffer     = _swapChain->GetFramebuffer(static_cast<int>(_currentImage)),
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
}

void VulkanContext::EndRenderPass() const
{
    GK_ASSERT(FrameInProgress(), "Failed to end render pass. Frame has not begun!")
    const auto buffer = GetCommandBuffer();
    buffer.endRenderPass();
    buffer.end();
}

void VulkanContext::SwapBuffers()
{
    GK_ASSERT(FrameInProgress(), "Failed to submit buffer. Frame has not begun!")
    try
    {
        // Command buffers to submit
         std::vector buffers { GetCommandBuffer() };

        // Record ImGUI
        if (!_vulkanUI.expired())
        {
            const vk::CommandBuffer& uiBuffer = _vulkanUI.lock()->Render(_currentImage);
            buffers.emplace_back(uiBuffer);
        }

        _inFrameRender = false;
        
        // Submit buffer to device graphics queue, and handle image swap based on present mode
        if (_swapChain->SubmitCommandBuffers(buffers, _currentImage) == vk::Result::eSuboptimalKHR)
        {
            Log::Warn("Sub-optimal result of presentation");
            return CreateSwapchain();
        }

        _currentFrame = (_currentFrame + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    }
    catch (vk::OutOfDateKHRError&)
    {
        // extent has changed, ie window resized/minimized
        return CreateSwapchain();
    }
    catch (vk::SystemError& err)
    {
        Log::Error("Failed to swap buffers: {}", err.what());
    }
}

void VulkanContext::Resize(unsigned width, unsigned height)
{
    if (_extent.width == width && _extent.height == height) return;

    _extent = { width, height };
    _extentWasResized = true;

    // Crate new swap chain here to get rendering on resize
    CreateSwapchain();
}

VulkanContext::~VulkanContext()
{
    // wait for rendering to complete
    _device->GetDevice().waitIdle();
    VulkanContext::Resize(0, 0);

    // call destructors
    _swapChain.reset();
    _device.reset();

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

    #ifdef GK_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif
    
    return extensions;
}

#ifdef GK_DEBUG
void VulkanContext::InitDebug()
{
    const auto& props = _device->GetPhysicalDevice().getProperties();

    // Print device info
    Log::Info("{0:<12} {1}", "Device:", props.deviceName);

    std::string type;
    switch (props.deviceType)
    {
    case vk::PhysicalDeviceType::eDiscreteGpu:
        type = "Discrete"; break;
    case vk::PhysicalDeviceType::eIntegratedGpu:
        type = "Integrated"; break;
    case vk::PhysicalDeviceType::eCpu:
        type = "CPU"; break;
    case vk::PhysicalDeviceType::eVirtualGpu:
        type = "Virtual"; break;
    case vk::PhysicalDeviceType::eOther:
        type = "Other";
    }
    Log::Info("{0:<12} {1}", "Type:", type);
    
    Log::Info("{0:<12} v{1}.{2}.{3}", "API:", VK_API_VERSION_MAJOR(props.apiVersion), VK_API_VERSION_MINOR(props.apiVersion), VK_API_VERSION_PATCH(props.apiVersion));
    Log::Info("{0:<12} v{1}.{2}.{3}", "Driver:", VK_API_VERSION_MAJOR(props.driverVersion), VK_API_VERSION_MINOR(props.driverVersion), VK_API_VERSION_PATCH(props.driverVersion));
}
#endif
