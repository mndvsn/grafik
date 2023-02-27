/**
 * Grafik
 * Lab: ClearColor
 * Copyright 2012-2022 Martin Furuberg
 */
#include "gpch.h"
#include "ClearColor.h"

#include "renderer/RenderCommand.h"

#include <imgui/imgui.h>


namespace labb
{
    LClearColor::LClearColor()
        : _color { 0.706f, 0.671f, 0.557f, 1.000f } { }

    void LClearColor::OnRender(RenderEvent& e)
    {
        RenderCommand::SetClearColor(_color);
        RenderCommand::ClearBuffer();

        e.Handled();
    }

    void LClearColor::OnUI(UIEvent& e)
    {
        LLab::OnUI(e);

        ImGui::Begin("Clear Color", &_keepAlive, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::ColorEdit4("Color", &_color.r);
        ImGui::End();
    }
}
