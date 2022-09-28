﻿/**
 * Grafik
 * Renderer
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Renderer.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/glm.hpp>


Renderer::Renderer(GLFWwindow* window)
{
    _context = window;
}

void Renderer::Render(const VertexArray& vao, const Shader& shader, const int elementStart, int elementEnd) const
{
    if (shader.Bind())
    {
        vao.Bind();

        if (!elementEnd)
        {
            elementEnd = vao.GetElementCount()-1;
        }
        const int count = elementEnd+1 - elementStart;
        const void* offset = reinterpret_cast<const void*>(static_cast<intptr_t>(sizeof(unsigned)*elementStart)); // NOLINT(performance-no-int-to-ptr)
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, offset);
    }
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(const glm::vec3& color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::SetWireframeMode(bool bUseLineDraw)
{
    glPolygonMode(GL_FRONT_AND_BACK, bUseLineDraw ? GL_LINE : GL_FILL);
}

bool Renderer::GetFramebufferSize(int& width, int& height) const
{
    if (!_context)
    {
        return false;
    }
    glfwGetFramebufferSize(_context, &width, &height);
    return true;
}

Renderer::~Renderer()
{
    _context = nullptr;
}
