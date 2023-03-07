/**
 * Grafik
 * RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "RendererAPI.h"

#include "renderer/opengl/OpenGLRendererAPI.h"
#include "renderer/vulkan/VulkanRendererAPI.h"


std::unique_ptr<RendererAPI> RendererAPI::Create(API api)
{
    _api = api;
    switch (_api)
    {
        case API::None:             return nullptr;
        case API::OpenGL:           return std::make_unique<OpenGLRendererAPI>();
        case API::Vulkan:           return std::make_unique<VulkanRendererAPI>();
    }
    return nullptr;
}

std::string_view RendererAPI::GetAPIString(API api)
{
    switch (api)
    {
        case API::None:       return "None";
        case API::OpenGL:     return "OpenGL";
        case API::Vulkan:     return "Vulkan";
    }
    return "None";
}
