/**
 * Grafik
 * Renderer
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Renderer.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glad/glad.h>


void Renderer::Render(const VertexArray& vao, const Shader& shader) const
{
    shader.Bind();
    vao.Bind();
    
    glDrawElements(GL_TRIANGLES, vao.GetElementCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetWireframeMode(bool bUseLineDraw)
{
    glPolygonMode(GL_FRONT_AND_BACK, bUseLineDraw ? GL_LINE : GL_FILL);
}
