/**
 * Grafik
 * RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "RendererAPI.h"

#include "renderer/opengl/OpenGLRendererAPI.h"


std::unique_ptr<RendererAPI> RendererAPI::Create(API api)
{
    _api = api;
    switch (_api)
    {
        case API::None:             return nullptr;
        case API::OpenGL:           return std::make_unique<OpenGLRendererAPI>();
        case API::Vulkan:           return nullptr;
    }
    return nullptr;
}
