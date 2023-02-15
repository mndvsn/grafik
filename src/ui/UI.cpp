﻿/**
 * Grafik
 * UI
 * Copyright 2023 Martin Furuberg 
 */
#include "UI.h"

#include "renderer/RendererAPI.h"
#include "ui/opengl/OpenGLUI.h"


std::unique_ptr<UI> UI::Create()
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:             return nullptr;
        case RendererAPI::API::OpenGL:           return std::make_unique<OpenGLUI>();
        case RendererAPI::API::Vulkan:           return nullptr;
    }
    return nullptr;
}
