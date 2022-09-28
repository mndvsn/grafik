﻿/**
 * Grafik
 * Lab: ClearColor
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "ClearColor.h"


namespace labb
{
    LClearColor::LClearColor(Renderer& rr)
        : LLab { rr }
        , _color { 0.706f, 0.671f, 0.557f, 1.000f }
    {
        
    }

    void LClearColor::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);
    }

    void LClearColor::BeginRender()
    {
        GetRenderer().SetClearColor(_color);
        GetRenderer().Clear();
    }

    void LClearColor::BeginGUI(bool* bKeep)
    {
        LLab::BeginGUI(bKeep);
        
        ImGui::Begin("Clear Color", bKeep, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::ColorEdit4("Color", &_color.r);
        ImGui::End();
    }
}