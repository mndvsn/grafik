﻿/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"


struct GLFWwindow;
class Event;

struct WindowProperties
{
    std::string title { };
    unsigned width { 640 };
    unsigned height { 480 };
};

using EventCallbackFunc = std::function<void(Event&)>;

class Window
{
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

    void SetEventCallback(const EventCallbackFunc& func) { _state.eventCallback = func; }
    void Close();

    [[nodiscard]] bool IsRunning() const { return _state.running; }
    [[nodiscard]] bool IsMinimized() const { return _state.minimized; }
    [[nodiscard]] GLFWwindow* GetSysWindow() const { return _window; }

protected:
    [[nodiscard]] std::string GetDetailedWindowTitle() const;
    static void glfwError(int error, const char* description);

    struct WindowState
    {
        std::string title { };
        unsigned width { 640 };
        unsigned height { 480 };
        bool running { false };
        bool minimized { false };
        EventCallbackFunc eventCallback { };

        void SetSize(unsigned _width, unsigned _height)
        {
            width = _width;
            height = _height;
            if (_width == 0 && _height == 0)
            {
                minimized = true;
                return;
            }
            minimized = false;
        }
    } _state;
};
