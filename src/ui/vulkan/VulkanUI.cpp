/**
 * Grafik
 * Vulkan UI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "VulkanUI.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanSwapChain.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>


void VulkanUI::Init(GLFWwindow* glfwWindow)
{
    const auto& app = Application::Get();
    _context = app.GetWindow()->GetContext<VulkanContext>();
    assert(_context && "Failed to get vulkan context!");

    _context->AttachUI(app.GetUI<VulkanUI>());
    _vulkanDevice = _context->GetDevice();

    // Create RenderPass, FrameBuffers, CommandBuffers
    CreateContext();
    CreateDescriptorPool();
    
    // Init ImGUI for GLFW, Vulkan
    auto initInfo = ImGui_ImplVulkan_InitInfo
    {
        .Instance = _context->GetInstance(),
        .PhysicalDevice = _vulkanDevice->GetPhysicalDevice(),
        .Device = _vulkanDevice->GetDevice(),
        .QueueFamily = _vulkanDevice->FindPhysicalQueueFamilies().graphicsFamily.value(),
        .Queue = _vulkanDevice->GetGraphicsQueue(),
        .PipelineCache = nullptr,
        .DescriptorPool = _descriptorPool,
        .Subpass = 0,
        .MinImageCount = _vulkanDevice->GetSwapChainSupport().capabilities.minImageCount,
        .ImageCount = static_cast<uint32_t>(_vulkanSwapchain->GetImageCount()),
        .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
        .Allocator = nullptr,
        //.CheckVkResultFn = // vk::Result check fn
    };

    ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);
    ImGui_ImplVulkan_Init(&initInfo, _renderPass);

    CreateFontTexture();
}

void VulkanUI::CreateContext()
{
    DestructVulkanObjects();
    
    _vulkanSwapchain = _context->GetSwapChain();
    const size_t imageCount = _vulkanSwapchain->GetImageCount();

    CreateRenderPass();

    // Framebuffers
    _framebuffers.resize(imageCount);
    for (size_t i = 0; i < imageCount; i++)
    {
        std::vector attachments { _vulkanSwapchain->GetImageView(static_cast<int>(i)) };
        _framebuffers[i] = _vulkanSwapchain->CreateFramebuffer( _renderPass, attachments);
    }

    // Allocate command buffers if needed 
    if (imageCount != _commandBuffers.size())
    {
        // old buffers freed in swapchain
        CreateCommandBuffers(static_cast<uint32_t>(imageCount));
    }

    // update only if ImGUI is initialized
    if (ImGui::GetIO().BackendRendererUserData)
    {
        const auto minImageCount = _vulkanDevice->GetSwapChainSupport().capabilities.minImageCount;
        ImGui_ImplVulkan_SetMinImageCount(minImageCount);
    }
}

void VulkanUI::Begin()
{
    // Begin ImGUI frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void VulkanUI::End()
{
    ImGui::Render();
}

vk::CommandBuffer& VulkanUI::Render(uint32_t imageIndex)
{
    // Record command buffer
    auto& commandBuffer = _commandBuffers.at(imageIndex);

    // Begin
    constexpr auto beginInfo = vk::CommandBufferBeginInfo
    {
        .flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse
    };
    if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to begin recording command buffer!" };
    }

    // Begin Render Pass
    const vk::Extent2D extent = _vulkanSwapchain->GetExtent();
    const vk::ClearValue color
    {
        .color           = { std::array { 0.0f, 0.0f, 0.0f, 1.0f } }
    };
    const std::vector clearValues { color };

    const vk::RenderPassBeginInfo renderPassBeginInfo
    {
        .renderPass      = _renderPass,
        .framebuffer     = _framebuffers[imageIndex],
        .renderArea      = {
            .offset      = { 0, 0 },
            .extent      = extent
        },
        .clearValueCount = static_cast<uint32_t>(clearValues.size()),
        .pClearValues    = clearValues.data()
    };
    commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

    // End
    commandBuffer.endRenderPass();
    commandBuffer.end();
    
    return commandBuffer;
}

void VulkanUI::CreateFontTexture()
{
    const vk::CommandBuffer cmdBuffer = _vulkanDevice->BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(cmdBuffer);
    _vulkanDevice->EndSingleTimeCommands(cmdBuffer);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VulkanUI::CreateRenderPass()
{
    const vk::AttachmentDescription colorAttachment
    {
        .format          = _vulkanSwapchain->GetImageFormat(),
        .samples         = vk::SampleCountFlagBits::e1,
        .loadOp          = vk::AttachmentLoadOp::eLoad,
        .storeOp         = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp   = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp  = vk::AttachmentStoreOp::eDontCare,
        .initialLayout   = vk::ImageLayout::eColorAttachmentOptimal,
        .finalLayout     = vk::ImageLayout::ePresentSrcKHR
    };

    constexpr vk::AttachmentReference colorAttachmentRef
    {
        .attachment      = 0,
        .layout          = vk::ImageLayout::eColorAttachmentOptimal
    };

    constexpr vk::SubpassDescription subpass
    {
        .pipelineBindPoint          = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount       = 1,
        .pColorAttachments          = &colorAttachmentRef
    };

    constexpr vk::SubpassDependency dependency
    {
        .srcSubpass             = VK_SUBPASS_EXTERNAL,
        .dstSubpass             = 0,
        .srcStageMask           = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask           = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask          = vk::AccessFlagBits::eColorAttachmentWrite,
        .dstAccessMask          = vk::AccessFlagBits::eColorAttachmentWrite,
    };

    const std::vector attachments = { colorAttachment };
    _renderPass = _vulkanSwapchain->CreateRenderPass(subpass, dependency, attachments);
}

void VulkanUI::CreateDescriptorPool()
{
    const std::vector<vk::DescriptorPoolSize> poolSizes =
    {
        { .type = vk::DescriptorType::eSampler,                 .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eCombinedImageSampler,    .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eSampledImage,            .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eStorageImage,            .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eUniformTexelBuffer,      .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eStorageTexelBuffer,      .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eUniformBuffer,           .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eStorageBuffer,           .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eUniformBufferDynamic,    .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eStorageBufferDynamic,    .descriptorCount = 1000 },
        { .type = vk::DescriptorType::eInputAttachment,         .descriptorCount = 1000 },
    };
    _descriptorPool = _vulkanDevice->CreateDescriptorPool(poolSizes, 1000);
}

void VulkanUI::CreateCommandBuffers(uint32_t count)
{
    _commandBuffers.clear();

    const vk::CommandBufferAllocateInfo allocInfo
    {
        .commandPool = _vulkanDevice->GetCommandPool(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = count
    };

    try
    {
        _commandBuffers = _vulkanDevice->GetDevice().allocateCommandBuffers(allocInfo);
    }
    catch (vk::SystemError&)
    {
        throw std::runtime_error { "Failed to allocate vulkan command buffers!" };
    }
}

void VulkanUI::DestructVulkanObjects() const
{
    // wait for rendering to complete
    _vulkanDevice->GetDevice().waitIdle();
    
    for (const auto& fb : _framebuffers)
    {
        _vulkanDevice->GetDevice().destroyFramebuffer(fb);
    }
    _vulkanDevice->GetDevice().destroyRenderPass(_renderPass);
}

VulkanUI::~VulkanUI()
{
    if (_context) _context->DetachUI();

    DestructVulkanObjects();
    _vulkanDevice->GetDevice().destroyDescriptorPool(_descriptorPool);
    
    ImGui_ImplVulkan_Shutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
