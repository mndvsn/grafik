﻿/**
 * Grafik
 * GLRender
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>

#include "RenderApp.h"


class GLRender: public RenderApp
{
public:
    GLRender(const char* title, int width, int height, const char* initLab);
    ~GLRender() override;

    void Init() override;
    void Setup() override;
    void Run() override;

private:
    void InitImGUI() override;
#ifdef _DEBUG
    void InitDebug() override;
#endif
};
