/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "core/ComponentManager.h"
#include "events/EventManager.h"
#include "renderer/RendererAPI.h"


class UI;
class Window;
class WindowCloseEvent;
class WindowSizeEvent;

struct ApplicationArgs
{
    int count { 0 };
    char** values { nullptr };

    const char* operator[](const int index) const
    {
        return values[index];
    }
};

struct ApplicationConfig
{
    std::string         title           { };
    unsigned            width           { 640 };
    unsigned            height          { 480 };
    RendererAPI::API    api             { RendererAPI::API::OpenGL };
    std::string         initLab         { };
    bool                wireFrameMode   { false };
    ApplicationArgs     args            { };
};

class Application
{
    ApplicationConfig _config;
    ComponentManager _components { };
    std::unique_ptr<Window> _window { nullptr };
    std::unique_ptr<UI> _ui { nullptr };
    inline static Application* _application { nullptr };
    
public:
    Application(ApplicationConfig config = ApplicationConfig());
    ~Application();
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Init();
    void Run();

    void OnEvent(Event& event) const;

    static Application& Get() { return *_application; }

    [[nodiscard]] Window* GetWindow() const { return _window.get(); }

private:
    void InitUI();

    void OnWindowClose(WindowCloseEvent& e) const;
    void OnWindowResize(const WindowSizeEvent& e) const;
    
    static void CheckArgs(ApplicationConfig& config);
};
