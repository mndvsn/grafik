/**
 * Grafik
 * GraphicsContext
 * Copyright 2023 Martin Furuberg 
 */
#include "GraphicsContext.h"

#include "renderer/RendererAPI.h"
#include "renderer/opengl/OpenGLContext.h"
#include "renderer/vulkan/VulkanContext.h"


std::unique_ptr<GraphicsContext> GraphicsContext::Create()
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:           return nullptr;
        case RendererAPI::API::OpenGL:         return std::make_unique<OpenGLContext>();
        case RendererAPI::API::Vulkan:         return std::make_unique<VulkanContext>();
    }
    return nullptr;
}
