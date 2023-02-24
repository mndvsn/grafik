/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg
 */
#include "gpch.h"
#include "Lab.h"

#include "renderer/RenderCommand.h"
#include "renderer/Shader.h"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <imgui/imgui.h>


namespace labb
{
    void LLab::OnAttach(int& eventMask)
    {
        eventMask = Event::Category::Application;
    }
    
    void LLab::OnEvent(Event& e)
    {
        EventDispatcher dispatcher { e };
        dispatcher.Dispatch<TickEvent>(GK_BIND_EVENT_HANDLER(OnTick));
        dispatcher.Dispatch<RenderEvent>(GK_BIND_EVENT_HANDLER(OnRender));
        dispatcher.Dispatch<UIEvent>(GK_BIND_EVENT_HANDLER(OnUI));
    }

    void LLab::OnTick(TickEvent&)
    {
    }

    void LLab::OnRender(RenderEvent&)
    {
        RenderCommand::SetClearColor({ 0.08f, 0.08f, 0.08f });
        RenderCommand::ClearBuffer();
    }

    void LLab::OnUI(UIEvent&)
    {
        if (_bHasError)
        {
            const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(main_viewport->GetCenter(), ImGuiCond_Always, { 0.5f, 0.5f });
            ImGui::SetNextWindowSizeConstraints({ 250, -1 }, { 450,0 });
            if (ImGui::Begin("Error", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::AlignTextToFramePadding();
                ImGui::PushTextWrapPos(450.0f);
                ImGui::Text("%s", _errorString.c_str());
                ImGui::End();
            }
        }
    }

    void LLab::RenderError(const std::string_view& error)
    {
        if (!_bHasError || _errorString != error)
        {
            _bHasError = true;
            _errorString = error;
        }
    }

    LLab::~LLab()
    {
        Shader::Unbind();
        if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
        {
            VertexArray::Unbind();
            VertexBuffer::Unbind();
        }
    }
}
