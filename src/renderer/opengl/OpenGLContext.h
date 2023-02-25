/**
 * Grafik
 * OpenGLContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"


struct GLFWwindow;

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext();

    void Init(GLFWwindow* window) override;
    void SwapBuffers() override;

    void SetState();

#ifdef _DEBUG
    void InitDebug() const;
#endif
};
