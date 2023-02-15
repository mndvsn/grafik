/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"

#include <memory>
#include <string>


struct GLFWwindow;

struct WindowProperties
{
    unsigned width { 640 };
    unsigned height { 480 };
    std::string title { };
};

class Window
{
    WindowProperties _props;
    GLFWwindow* _window { nullptr };
    std::unique_ptr<GraphicsContext> _context { nullptr };

public:
    Window(const WindowProperties& prop = WindowProperties());
    virtual ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void Init();
    void CreateWindow();

    void Update();

    [[nodiscard]] bool IsRunning() const;
    [[nodiscard]] GLFWwindow* GetSysWindow() const { return _window; }
    [[nodiscard]] const WindowProperties* GetProps() const { return &_props; }
    
    void Shutdown();

protected:
    static void glfwError(int error, const char* description);
};
