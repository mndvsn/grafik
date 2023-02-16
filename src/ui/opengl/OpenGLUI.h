/**
 * Grafik
 * OpenGL UI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "ui/UI.h"


class OpenGLUI : public UI
{
public:
    ~OpenGLUI() override;
    
    void Init(GLFWwindow* window) override;

    void Begin() override;
    void End() override;
};
