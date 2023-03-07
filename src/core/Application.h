/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "core/ComponentManager.h"
#include "events/EventManager.h"
#include "events/ApplicationEvent.h"
#include "renderer/RendererAPI.h"
#include "ui/UI.h"


class Window;
namespace labb { class LLabMenu; }

class Application
{
public:
    struct Args
    {
        int count { 0 };
        char** values { nullptr };

        const char* operator[](const int index) const
        {
            return values[index];
        }
    };
    
    struct Config
    {
        std::string         title           { };
        unsigned            width           { 640 };
        unsigned            height          { 480 };
        RendererAPI::API    api             { RendererAPI::API::OpenGL };
        std::string         initLab         { };
        bool                wireFrameMode   { false };
        Args                args            { };
    };
    
    Config _config;
    ComponentManager _components { };
    Window* _window { };
    labb::LLabMenu* _menu { };
    std::unique_ptr<UI> _ui { };
    inline static Application* _application { };
    
    Application(Config config = Config());
    ~Application();
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(const Application&&) = delete;
    Application& operator=(const Application&&) = delete;

    void Init();
    void Run();

    void OnEvent(Event& event);

    static Application& Get() { return *_application; }

    [[nodiscard]] Window* GetWindow() const { return _window; }

private:
    void InitUI();
    void InitLabs();

    void OnWindowClose(WindowCloseEvent& e) const;
    void OnWindowResize(const WindowSizeEvent& e) const;
    void OnFramebufferSize(const FramebufferSizeEvent& e) const;
    
    void OnInitLab(InitLabEvent& e);
    
    static void CheckArgs(Config& config);
};
