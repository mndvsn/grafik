/**
 * Grafik
 * Vulkan: Swap chain
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanSwapChain.h"

#include "core/Application.h"
#include "components/Window.h"


VulkanSwapChain::VulkanSwapChain(VulkanDevice& device)
    : _device { device }
{
    Init();
    CreateImageViews();
    CreateDepthResources();
    CreateRenderPass();
    CreateFramebuffers();
    CreateSyncObjects();
}

void VulkanSwapChain::Init()
{
    const SwapChainSupportDetails supported = _device.GetSwapChainSupport();

    const auto surfaceFormat = SelectSurfaceFormat(supported.formats);
    const auto presentMode = SelectPresentMode(supported.presentModes);
    _extent = SelectImageExtent(supported.capabilities);

    uint32_t imageCount = supported.capabilities.minImageCount + 1;
    if (supported.capabilities.maxImageCount > 0 && imageCount > supported.capabilities.maxImageCount)
    {
        imageCount = supported.capabilities.maxImageCount;
    }

     auto swapchainInfo = vk::SwapchainCreateInfoKHR
    {
        .surface            = _device.GetSurface(),
        .minImageCount      = imageCount,
        .imageFormat        = surfaceFormat.format,
        .imageColorSpace    = surfaceFormat.colorSpace,
        .imageExtent        = _extent,
        .imageArrayLayers   = 1,
                            // render directly to image
        .imageUsage         = vk::ImageUsageFlagBits::eColorAttachment 
    };

    // Get index of queues on physical device
    const QueueFamilyIndices indices = _device.FindPhysicalQueueFamilies();
    if (!indices.graphicsFamily || !indices.presentFamily)
    {
        throw std::runtime_error { "Failed to get vulkan physical device queue families!" };
    }
    const uint32_t queueFamilyIndices[] { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily.value() != indices.presentFamily.value())
    {
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapchainInfo.queueFamilyIndexCount = 2;
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    // Optionally apply image transform
    swapchainInfo.preTransform = supported.capabilities.currentTransform;
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapchainInfo.presentMode = presentMode;
    swapchainInfo.clipped = true;
    swapchainInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

    try
    {
        _swapChain = _device.GetDevice().createSwapchainKHR(swapchainInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Vulkan swap chain creation failed: " << error.what() << std::endl;
    }

    _images = _device.GetDevice().getSwapchainImagesKHR(_swapChain);
    _imageFormat = surfaceFormat.format;
}

void VulkanSwapChain::CreateImageViews()
{
    _imageViews.resize(_images.size());

    for (size_t i = 0; i < _images.size(); i++)
    {
        auto viewInfo = vk::ImageViewCreateInfo
        {
            .image              = _images[i],
            .viewType           = vk::ImageViewType::e2D,
            .format             = _imageFormat,
            .subresourceRange   = {
                .aspectMask     = vk::ImageAspectFlagBits::eColor,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };

        try
        {
            _imageViews[i] = _device.GetDevice().createImageView(viewInfo);
        }
        catch (vk::SystemError& err)
        {
            std::cerr << "Failed to create vulkan image view: " << err.what() << std::endl;
        }
    }
}

void VulkanSwapChain::CreateDepthResources()
{
    const auto depthFormat = FindDepthFormat();

    // Create equal amount of depth images as for color 
    const size_t count = GetImageCount();
    _depthImages.resize(count);
    _depthImageViews.resize(count);
    _depthImageMemory.resize(count);

    for (size_t i = 0; i < _depthImages.size(); i++)
    {
        vk::ImageCreateInfo imageInfo
        {
            .imageType          = vk::ImageType::e2D,
            .format             = depthFormat,
            .extent = {
                .width          = _extent.width,
                .height         = _extent.height,
                .depth          = 1
            },
            .mipLevels          = 1,
            .arrayLayers        = 1,
            .samples            = vk::SampleCountFlagBits::e1,
            .tiling             = vk::ImageTiling::eOptimal,
            .usage              = vk::ImageUsageFlagBits::eDepthStencilAttachment,
            .sharingMode        = vk::SharingMode::eExclusive,
            .initialLayout      = vk::ImageLayout::eUndefined,
        };

        _device.CreateImage(imageInfo, vk::MemoryPropertyFlagBits::eDeviceLocal, _depthImages[i], _depthImageMemory[i]);

        vk::ImageViewCreateInfo viewInfo
        {
            .image              = _depthImages[i],
            .viewType           = vk::ImageViewType::e2D,
            .format             = depthFormat,
            .subresourceRange   = {
                .aspectMask     = vk::ImageAspectFlagBits::eDepth,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };

        try
        {
            _depthImageViews[i] = _device.GetDevice().createImageView(viewInfo);
        }
        catch (vk::SystemError& err)
        {
            std::cerr << "Failed to create vulkan image view: " << err.what() << std::endl;
        }
    }
}

void VulkanSwapChain::CreateRenderPass()
{
    const vk::AttachmentDescription colorAttachment
    {
        .format          = _imageFormat,
        .samples         = vk::SampleCountFlagBits::e1,
        .loadOp          = vk::AttachmentLoadOp::eClear,
        .storeOp         = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp   = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp  = vk::AttachmentStoreOp::eDontCare,
        .initialLayout   = vk::ImageLayout::eUndefined,
        .finalLayout     = vk::ImageLayout::ePresentSrcKHR
    };

    /* Index of attachment
     * GLSL: layout(location = 0) out vec4 outColor */
    vk::AttachmentReference colorAttachmentRef
    {
        .attachment      = 0,
        .layout          = vk::ImageLayout::eColorAttachmentOptimal
    };

    const vk::AttachmentDescription depthAttachment
    {
        .format          = FindDepthFormat(),
        .samples         = vk::SampleCountFlagBits::e1,
        .loadOp          = vk::AttachmentLoadOp::eClear,
        .storeOp         = vk::AttachmentStoreOp::eDontCare,
        .stencilLoadOp   = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp  = vk::AttachmentStoreOp::eDontCare,
        .initialLayout   = vk::ImageLayout::eUndefined,
        .finalLayout     = vk::ImageLayout::eDepthStencilAttachmentOptimal
    };

    vk::AttachmentReference depthAttachmentRef
    {
        .attachment      = 1,
        .layout          = vk::ImageLayout::eDepthStencilAttachmentOptimal
    };

    vk::SubpassDescription subpass
    {
        .pipelineBindPoint          = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount       = 1,
        .pColorAttachments          = &colorAttachmentRef,
        .pDepthStencilAttachment    = &depthAttachmentRef
    };

    vk::SubpassDependency dependency
    {
        .srcSubpass             = VK_SUBPASS_EXTERNAL,
        .dstSubpass             = 0,
        .srcStageMask           = vk::PipelineStageFlagBits::eColorAttachmentOutput
                                | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .dstStageMask           = vk::PipelineStageFlagBits::eColorAttachmentOutput
                                | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .srcAccessMask          = vk::AccessFlagBits::eNone,
        .dstAccessMask          = vk::AccessFlagBits::eColorAttachmentWrite
                                | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
    };

    const std::array attachments = { colorAttachment, depthAttachment };

    const vk::RenderPassCreateInfo renderPassInfo
    {
        .attachmentCount        = static_cast<uint32_t>(attachments.size()),
        .pAttachments           = attachments.data(),
        .subpassCount           = 1,
        .pSubpasses             = &subpass,
        .dependencyCount        = 1,
        .pDependencies          = &dependency
    };

    try
    {
        _renderPass = _device.GetDevice().createRenderPass(renderPassInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create vulkan Render Pass: " << error.what() << std::endl;
    }
}

void VulkanSwapChain::CreateFramebuffers()
{
    const size_t count = GetImageCount();
    _framebuffers.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        std::array attachments { _imageViews[i], _depthImageViews[i] };

        const vk::FramebufferCreateInfo framebufferInfo
        {
            .renderPass = _renderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = _extent.width,
            .height = _extent.height,
            .layers = 1
        };

        try
        {
            _framebuffers[i] = _device.GetDevice().createFramebuffer(framebufferInfo);
        }
        catch (vk::SystemError& err)
        {
            std::cerr << "Failed to create vulkan framebuffer: " << err.what() << std::endl;
        }
    }
}

void VulkanSwapChain::CreateSyncObjects()
{
    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    _imagesInFlight.resize(GetImageCount(), nullptr);

    constexpr auto fenceInfo = vk::FenceCreateInfo { .flags = vk::FenceCreateFlagBits::eSignaled };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        try
        {
            _imageAvailableSemaphores[i] = _device.GetDevice().createSemaphore({ });
            _renderFinishedSemaphores[i] = _device.GetDevice().createSemaphore({ });
            _inFlightFences[i] = _device.GetDevice().createFence(fenceInfo);
        }
        catch (vk::SystemError& error)
        {
            std::cerr << "Failed to create vulkan swap chain synchronization objects: " << error.what() << std::endl;
        }
    }
}

/* -------------------------- */

bool VulkanSwapChain::GetNextImage(uint32_t& imageIndex) const
{
    if (_device.GetDevice().waitForFences(1, &_inFlightFences[_currentFrame], true, std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Error waiting for vulkan swap chain fence!" };
    }
    try
    {
        imageIndex = _device.GetDevice().acquireNextImageKHR(_swapChain, std::numeric_limits<uint64_t>::max(), _imageAvailableSemaphores[_currentFrame], nullptr).value;
    }
    catch (vk::OutOfDateKHRError& error)
    {
        std::cerr << "Vulkan image out of date: " << error.what() << std::endl;
        return false;
    }
    catch (vk::SystemError&)
    {
        throw std::runtime_error { "Failed to get next vulkan swap chain image!" };
    }
    return true;
}

void VulkanSwapChain::SubmitCommandBuffers(const vk::CommandBuffer* buffers, const uint32_t* imageIndex)
{
    if (_imagesInFlight[*imageIndex] != vk::Fence(nullptr))
    {
        if (_device.GetDevice().waitForFences(1, &_inFlightFences[_currentFrame], true, std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess)
        {
            throw std::runtime_error { "Failed to wait on vulkan swap chain" };
        }
    }
    _imagesInFlight[*imageIndex] = _inFlightFences[_currentFrame];

    const vk::Semaphore waitSemaphores[] { _imageAvailableSemaphores[_currentFrame] };
    const vk::Semaphore signalSemaphores[] { _renderFinishedSemaphores[_currentFrame] };

    constexpr vk::PipelineStageFlags waitStages[] { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    const vk::SubmitInfo submitInfo
    {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = buffers,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores
    };

    if (_device.GetDevice().resetFences(1, &_inFlightFences[_currentFrame]) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to reset vulkan swap chain image!" };
    }

    try
    {
        _device.GetGraphicsQueue().submit(submitInfo, _inFlightFences[_currentFrame]);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to submit vulkan command buffer: " << error.what() << std::endl;
    }

    const vk::SwapchainKHR swapChains[] = { _swapChain };

    const auto presentInfo = vk::PresentInfoKHR
    {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = imageIndex
    };

    if (_device.GetPresentQueue().presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to present vulkan command buffer!" };
    }

    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

vk::SurfaceFormatKHR VulkanSwapChain::SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
{
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
    {
        return { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
    }
    
    for (const auto& available : availableFormats)
    {
        if (available.format == vk::Format::eB8G8R8A8Srgb && available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return available;
        }
    }
    return availableFormats[0];
}

vk::PresentModeKHR VulkanSwapChain::SelectPresentMode(const std::vector<vk::PresentModeKHR>& availableModes) const
{
    // FIFO: V-Sync, wait for surface vertical blank (vblank)
    vk::PresentModeKHR preferredMode = vk::PresentModeKHR::eFifo;
    
    for (const auto& mode : availableModes)
    {
        // Mailbox: Lower latency, newer frame used, but higher GPU usage
        if (mode == vk::PresentModeKHR::eMailbox)
        {
            return mode;
        }
        // Immediate: May result in tearing
        if (mode == vk::PresentModeKHR::eImmediate)
        {
            preferredMode = mode;
        }
    }
    return preferredMode;
}

vk::Extent2D VulkanSwapChain::SelectImageExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
{
    // Check if extent is determined by the window manager
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) // 0xFFFFFFFF
    {
        return capabilities.currentExtent;
    }
    
    // Ask application about current window framebuffer size
    vk::Extent2D extent = GetWindowExtent();
    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return extent;
}

vk::Extent2D VulkanSwapChain::GetWindowExtent()
{
    auto[width, height] = Application::Get().GetWindow()->GetContextSize();
    const vk::Extent2D extent { width, height };
    return extent;
}

vk::Format VulkanSwapChain::FindDepthFormat() const
{
    return _device.FindSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

void VulkanSwapChain::Shutdown()
{
    const vk::Device& device = _device.GetDevice();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        device.destroySemaphore(_imageAvailableSemaphores[i]);
        device.destroySemaphore(_renderFinishedSemaphores[i]);
        device.destroyFence(_inFlightFences[i]);
    }
    
    for (const auto& framebuffer : _framebuffers)
    {
        device.destroyFramebuffer(framebuffer);
    }

    for (const auto& imageView : _imageViews)
    {
        device.destroyImageView(imageView);
    }
    _imageViews.clear();

    device.destroySwapchainKHR(_swapChain);

    for (size_t i = 0; i < _depthImages.size(); i++)
    {
        device.destroyImageView(_depthImageViews[i]);
        device.destroyImage(_depthImages[i]);
        device.freeMemory(_depthImageMemory[i]);
    }

    device.destroyRenderPass(_renderPass);
}
