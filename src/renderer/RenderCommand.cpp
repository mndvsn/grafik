/**
 * Grafik
 * RenderCommand
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "RenderCommand.h"

#include <glm/glm.hpp>


void RenderCommand::Init(const RendererAPI::API api)
{
    _renderAPI = RendererAPI::Create(api);
}

void RenderCommand::BeginFrame()
{
    _renderAPI->BeginFrame();
}

void RenderCommand::EndFrame()
{
    _renderAPI->EndFrame();
}

void RenderCommand::SetClearColor(const glm::vec3& color)
{
    _renderAPI->SetClearColor(color.r, color.g, color.b);
}

void RenderCommand::SetClearColor(const glm::vec4& color)
{
    _renderAPI->SetClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::ClearBuffer()
{
    _renderAPI->ClearBuffer();
}

void RenderCommand::SetWireframeMode(bool bUseLineDraw)
{
    _renderAPI->SetWireframeMode(bUseLineDraw);
}

void RenderCommand::SetViewport(int width, int height)
{
    _renderAPI->SetViewport(width, height);
}
