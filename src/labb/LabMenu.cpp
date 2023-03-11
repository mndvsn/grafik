/**
 * Grafik
 * LabMenu
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "LabMenu.h"

#include "core/Application.h"

#include <imgui/imgui.h>

#include <ranges>

#include "renderer/Renderer.h"


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
            if (ImGui::BeginMenu("Renderer"))
            {
                BeginRendererMenu();
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
            for (const auto&[name, createFunc] : _labs | std::views::values)
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    EventManager::Get()->Broadcast<InitLabEvent>(createFunc);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "ESC"))
        {
            EventManager::Get()->Broadcast<WindowCloseEvent>();
        }
    }

    void LLabMenu::BeginRendererMenu()
    {
        if (ImGui::BeginMenu("API"))
        {
            for (const auto api : RendererAPI::APIs)
            {
                if (ImGui::MenuItem(std::string(RendererAPI::GetAPIString(api)).c_str(),
                    nullptr, RendererAPI::GetAPI() == api))
                {
                    if (RendererAPI::GetAPI() == api) break;
                    
                    Grafik::APIOverride = static_cast<unsigned>(api);
                    EventManager::Get()->Broadcast<WindowCloseEvent>(true); // true = restart
                }
            }
            ImGui::EndMenu();
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
                for (const auto&[name, createFunc] : _labs | std::views::values)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::Button(name.c_str(), ImVec2(-FLT_MIN, 0.0f)))
                    {
                        EventManager::Get()->Broadcast<InitLabEvent>(createFunc);
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
