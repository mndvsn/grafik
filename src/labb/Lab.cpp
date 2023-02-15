/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Lab.h"

#include "core/Application.h"
#include "core/Window.h"
#include "renderer/RenderCommand.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include <ranges>


namespace labb
{
    void LLab::BeginRender()
    {
        RenderCommand::SetClearColor({ 0.08f, 0.08f, 0.08f });
        RenderCommand::ClearBuffer();
    }

    void LLab::BeginGUI(bool* bKeep)
    {
        (void)bKeep;

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

    void LLab::BeginUpdate(double DeltaTime)
    {
        (void)DeltaTime;
    }

    void LLab::RenderError(const std::string& error)
    {
        if (!_bHasError || _errorString != error)
        {
            _bHasError = true;
            _errorString = error;
        }
    }

    LLabMenu::LLabMenu(std::unique_ptr<LLab>& activeLabPtr)
        : _activeLab { activeLabPtr }
    {
        
    }

    void LLabMenu::BeginRender()
    {
        LLab::BeginRender();
    }

    void LLabMenu::BeginGUI(bool* bKeep)
    {
        LLab::BeginGUI(bKeep);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                BeginLabMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void LLabMenu::BeginLabMenu()
    {
        if (ImGui::BeginMenu("Open"))
        {
            for (const auto& lab : _labs | std::views::values)
            {
                if (ImGui::MenuItem(lab.name.c_str()))
                {
                    _activeLab.reset(lab.createInstance());
                }
            }
            ImGui::EndMenu();
        }
        if (GLFWwindow* window = Application::Get().GetWindow()->GetSysWindow())
        {
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "ESC"))
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
    }

    void LLabMenu::BeginBigMenu()
    {
        constexpr float padding { 30.0f };
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Always, { 0.5f, 0.5f });
        ImGui::SetNextWindowSizeConstraints({ 200.0f, 50.0f }, { viewport->WorkSize.x - padding,viewport->WorkSize.y - padding });
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::SetNextWindowSize(ImVec2(300.0f, 0.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 10.0f));
        
        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        if (ImGui::Begin("Labs", nullptr, flags))
        {
            if (ImGui::BeginTable("labsList", 1))
            {
                for (const auto& lab : _labs | std::views::values)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::Button(lab.name.c_str(), ImVec2(-FLT_MIN, 0.0f)))
                    {
                        _activeLab.reset(lab.createInstance());
                    }
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }
        ImGui::PopStyleVar(4);
    }

    std::optional<LLab*> LLabMenu::CreateLabIfExists(const std::string& labShortName)
    {
        auto matchesShortName = [labShortName](auto& labItem)
        {
            return labItem.first == labShortName;
        };
        
        const auto labItem = std::ranges::find_if(_labs, matchesShortName);
        if (labItem != _labs.end())
        {
            return labItem->second.createInstance();
        }
        return {};
    }

    LLab::~LLab()
    {
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }
}
