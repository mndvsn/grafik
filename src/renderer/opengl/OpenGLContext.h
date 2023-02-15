/**
 * Grafik
 * OpenGLContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
