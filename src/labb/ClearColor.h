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
        LClearColor();

        void OnRender(RenderEvent& e) override;
        void OnUI(UIEvent& e) override;
    };
}
