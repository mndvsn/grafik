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
#include <imgui/imgui.h>


void Renderer::Init(RendererAPI::API api)
{
    RenderCommand::Init(api);
}

void Renderer::Render()
{
    if (!IsRenderValid()) return;
    
    RenderCommand::BeginFrame();

    // Render components
    EventManager::Get()->Broadcast<RenderEvent>();
    
    // Render UI
    if (const auto ui = Application::Get().GetUI<UI>().lock())
    {
        ui->Begin();
        EventManager::Get()->Broadcast<UIEvent>();
        ui->End();
    }

    // Present frame
    RenderCommand::EndFrame();
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{
}

bool Renderer::IsRenderValid()
{
    int width, height;
    GetFramebufferSize(width, height);
    return (width > 0 && height > 0);
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

void Renderer::SetViewport(int width, int height)
{
    RenderCommand::SetViewport(width, height);
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
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
