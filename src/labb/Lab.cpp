/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Lab.h"


namespace labb
{
    LLab::LLab(Renderer& renderer)
        : _renderer { renderer }
    {
        
    }

    void LLab::BeginRender()
    {
        _renderer.SetClearColor({ 0.0f, 0.0f, 0.0f });
        _renderer.Clear();
    }

    void LLab::BeginGUI(bool* bKeep)
    {
        (void)bKeep;
    }

    void LLab::BeginUpdate(double DeltaTime)
    {
        (void)DeltaTime;
    }

    LLabMenu::LLabMenu(Renderer& renderer, LLab*& activeLabPtr)
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
                    delete _activeLab;
                    _activeLab = lab.createInstance();
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
