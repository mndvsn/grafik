/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "events/Event.h"
#include "renderer/RendererAPI.h"


class Window;
class UI;

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
    std::string title { };
    unsigned width { 640 };
    unsigned height { 480 };
    RendererAPI::API api { RendererAPI::API::OpenGL };
    std::string initLab { };
    ApplicationArgs args { };
};

class Application
{
    ApplicationConfig _config;
    std::unique_ptr<Window> _window { nullptr };
    std::unique_ptr<UI> _ui { nullptr };
    inline static Application* _application { nullptr };
    
public:
    Application(ApplicationConfig config = ApplicationConfig());
    ~Application();
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Init();
    void Run() const;

    void OnEvent(Event& event) const;

    static Application& Get() { return *_application; }

    [[nodiscard]] Window* GetWindow() const { return _window.get(); }

private:
    void InitUI();
    static void CheckArgs(ApplicationConfig& config);
};
