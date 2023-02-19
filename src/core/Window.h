/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"


struct GLFWwindow;
class Event;

using EventCallbackFunc = std::function<void(Event&)>;

struct WindowProperties
{
    unsigned width { 640 };
    unsigned height { 480 };
    std::string title { };
    EventCallbackFunc eventCallback { };
};

class Window
{
    WindowProperties _props;
    GLFWwindow* _window { nullptr };
    std::unique_ptr<GraphicsContext> _context { nullptr };

public:
    Window(const WindowProperties& prop = WindowProperties());
    virtual ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Init();
    void CreateNativeWindow();

    void Update();

    void SetEventCallback(const EventCallbackFunc& func) { _props.eventCallback = func; }

    [[nodiscard]] bool IsRunning() const;
    [[nodiscard]] GLFWwindow* GetSysWindow() const { return _window; }
    [[nodiscard]] const WindowProperties* GetProps() const { return &_props; }
    
    void Shutdown();

protected:
    static void glfwError(int error, const char* description);
};
