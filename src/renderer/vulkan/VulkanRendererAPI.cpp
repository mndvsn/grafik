/**
 * Grafik
 * Vulkan RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "VulkanRendererAPI.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/vulkan/VulkanContext.h"


void VulkanRendererAPI::BeginFrame() const
{
    const auto context = Application::Get().GetWindow()->GetContext<VulkanContext>();
    if (const auto commandBuffer = context->BeginFrame())
    {
        context->BeginRenderPass(commandBuffer);
    }
}

void VulkanRendererAPI::EndFrame() const
{
    const auto context = Application::Get().GetWindow()->GetContext<VulkanContext>();
    context->EndRenderPass();
    context->SwapBuffers();
}

void VulkanRendererAPI::ClearBuffer() const
{
}

void VulkanRendererAPI::SetClearColor(const float r, const float g, const float b, const float alpha)
{
    (void)(alpha+r+g+b);
}

void VulkanRendererAPI::SetWireframeMode(bool bUseLineDraw)
{
    (void)bUseLineDraw;
}

void VulkanRendererAPI::SetViewport(int, int)
{
}
