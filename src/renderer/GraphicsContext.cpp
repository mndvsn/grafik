/**
 * Grafik
 * GraphicsContext
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "GraphicsContext.h"

#include "renderer/RendererAPI.h"
#include "renderer/opengl/OpenGLContext.h"
#include "renderer/vulkan/VulkanContext.h"


std::shared_ptr<GraphicsContext> GraphicsContext::Create()
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:           return nullptr;
        case RendererAPI::API::OpenGL:         return std::make_shared<OpenGLContext>();
        case RendererAPI::API::Vulkan:         return std::make_shared<VulkanContext>();
    }
    return nullptr;
}
