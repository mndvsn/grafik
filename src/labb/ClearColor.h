/**
 * Grafik
 * Lab: ClearColor
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"


namespace labb
{
    class LClearColor : public LLab
    {
        glm::vec4 _color;
    
    public:
        LClearColor(Renderer& rr);
        ~LClearColor() override;

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;
    };
}
