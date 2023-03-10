/**
 * Grafik
 * OpenGL RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "OpenGLRendererAPI.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/opengl/OpenGLContext.h"

#include <glad/glad.h>


void OpenGLRendererAPI::BeginFrame() const
{
}

void OpenGLRendererAPI::EndFrame() const
{
    const auto context = Application::Get().GetWindow()->GetContext<OpenGLContext>();
    context->SwapBuffers();
}

void OpenGLRendererAPI::ClearBuffer() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::SetClearColor(const float r, const float g, const float b, const float alpha)
{
    glClearColor(r, g, b, alpha);
}

void OpenGLRendererAPI::SetWireframeMode(bool bUseLineDraw)
{
    glPolygonMode(GL_FRONT_AND_BACK, bUseLineDraw ? GL_LINE : GL_FILL);
}

void OpenGLRendererAPI::SetViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}
