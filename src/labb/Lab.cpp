/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Lab.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"


namespace labb
{
    LLab::LLab(Renderer& renderer)
        : _renderer { renderer }
    {
        
    }

    LLab::~LLab()
    {
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }

    void LLab::BeginRender()
    {
        _renderer.SetClearColor({ 0.0f, 0.0f, 0.0f });
        _renderer.Clear();
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

    LLabMenu::LLabMenu(Renderer& renderer, std::unique_ptr<LLab>& activeLabPtr)
        : LLab { renderer },
        _activeLab { activeLabPtr }
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
            for (auto& lab : _labs)
            {
                if (ImGui::MenuItem(lab.name.c_str()))
                {
                    _activeLab.reset(lab.createInstance());
                }
            }
            ImGui::EndMenu();
        }
        if (GLFWwindow* window = _renderer.GetContext())
        {
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "ESC"))
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
    }
}
