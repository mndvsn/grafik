/**
 * Grafik
 * Vulkan RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "VulkanRendererAPI.h"


void VulkanRendererAPI::ResetState() const
{
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
