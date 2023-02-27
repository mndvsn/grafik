/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "components/Component.h"
#include "renderer/GraphicsContext.h"


struct GLFWwindow;

struct WindowProperties
{
    std::string title { };
    unsigned width { 640 };
    unsigned height { 480 };
};

class Window : public Component
{
    GLFWwindow* _window { nullptr };
    std::unique_ptr<GraphicsContext> _context { nullptr };

public:
    Window(const WindowProperties& prop = WindowProperties());
    ~Window() override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void OnAttach(int& eventMask) override;
    void CreateNativeWindow();

    void Update();
    void Close();

    [[nodiscard]] bool IsRunning() const { return _state.running; }
    [[nodiscard]] bool IsMinimized() const { return _state.minimized; }
    [[nodiscard]] GLFWwindow* GetNativeWindow() const { return _window; }
    [[nodiscard]] std::pair<unsigned, unsigned> GetContextSize() const { return _context->GetSize(); }

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
