﻿/**
 * Grafik
 * LabMenu
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "LabMenu.h"

#include "core/Application.h"

#include <imgui/imgui.h>

#include <ranges>


namespace labb
{
    void LLabMenu::OnUI(UIEvent& e)
    {
        LLab::OnUI(e);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                BeginLabMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Draw selection window
        if (_bBigMenu) BeginBigMenu();
    }

    void LLabMenu::BeginLabMenu()
    {
        if (ImGui::BeginMenu("Open"))
        {
            for (const auto& lab : _labs | std::views::values)
            {
                if (ImGui::MenuItem(lab.name.c_str()))
                {
                    InitLabEvent event { lab.createInstance };
                    EventManager::Get()->Broadcast(event);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "ESC"))
        {
            WindowCloseEvent e;
            EventManager::Get()->Broadcast(e);
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
                        InitLabEvent event { lab.createInstance };
                        EventManager::Get()->Broadcast(event);
                    }
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }
        ImGui::PopStyleVar(4);
    }

    void LLabMenu::CreateLabIfExists(const std::string& labShortName)
    {
        auto matchesShortName = [labShortName](auto& labItem)
        {
            return labItem.first == labShortName;
        };
        
        const auto labItem = std::ranges::find_if(_labs, matchesShortName);
        if (labItem != _labs.end())
        {
            InitLabEvent event { labItem->second.createInstance };
            EventManager::Get()->Broadcast(event);
        }
    }
}
