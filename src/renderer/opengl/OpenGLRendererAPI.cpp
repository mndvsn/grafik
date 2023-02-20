/**
 * Grafik
 * OpenGL RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>


void OpenGLRendererAPI::ResetState() const
{
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
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
