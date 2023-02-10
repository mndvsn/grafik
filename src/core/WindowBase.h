﻿/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include <GLFW/glfw3.h>
#include <string>


class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual void Init(const std::string& title, int width, int height) = 0;
    virtual void Setup() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    
    virtual void BeginImGUI() const = 0;
    virtual void RenderImGUI() const = 0;

    [[nodiscard]] virtual GLFWwindow* GetWindow() const = 0;
    [[nodiscard]] virtual bool IsRunning() const = 0;
    
    virtual void Shutdown() = 0;

protected:
    virtual void CreateWindow(const std::string& title, int width, int height) = 0;
    virtual void InitImGUI() = 0;

#ifdef _DEBUG
    virtual void InitDebug() = 0;
#endif
};

class Window
{
protected:
    GLFWwindow* _window { nullptr };

public:
    Window() = default;
    virtual ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    virtual void InitGLFW();

protected:
    static void glfwError(int error, const char* description);
};
