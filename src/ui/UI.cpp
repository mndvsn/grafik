/**
 * Grafik
 * UI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "UI.h"

#include "renderer/RendererAPI.h"
#include "ui/opengl/OpenGLUI.h"
#include "ui/vulkan/VulkanUI.h"


std::shared_ptr<UI> UI::Create()
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:             return nullptr;
        case RendererAPI::API::OpenGL:           return std::make_shared<OpenGLUI>();
        case RendererAPI::API::Vulkan:           return std::make_shared<VulkanUI>();
    }
    return nullptr;
}
