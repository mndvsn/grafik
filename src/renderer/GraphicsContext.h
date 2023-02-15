/**
 * Grafik
 * GraphicsContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include <memory>


struct GLFWwindow;

class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;
    
    virtual void Init(GLFWwindow* window) = 0;
    virtual void SwapBuffers() = 0;

    static std::unique_ptr<GraphicsContext> Create();

protected:
    GLFWwindow* _window { nullptr };
};
