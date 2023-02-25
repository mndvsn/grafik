/**
 * Grafik
 * Renderer
 * Copyright Martin Furuberg 
 */
#include "gpch.h"
#include "Renderer.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/RenderCommand.h"
#include "renderer/Shader.h"

#include "VertexArray.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


Renderer::Renderer(GLFWwindow* window)
{
    _context = window;
}

void Renderer::Render(const VertexArray& vao, const std::shared_ptr<Shader>& shader, const int elementStart, int elementEnd)
{
    if (shader->Bind())
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

void Renderer::Init(RendererAPI::API api)
{
    RenderCommand::Init(api);
}

void Renderer::BeginFrame()
{
    RenderCommand::ResetState();
}

void Renderer::EndFrame()
{
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{
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

void Renderer::SetViewport(int width, int height)
{
    RenderCommand::SetViewport(width, height);
}

bool Renderer::GetFramebufferSize(int& width, int& height)
{
    GLFWwindow* _window = Application::Get().GetWindow()->GetNativeWindow();
    
    if (!_window)
    {
        return false;
    }
    glfwGetFramebufferSize(_window, &width, &height);
    return true;
}

Renderer::~Renderer()
{
    _context = nullptr;
}
