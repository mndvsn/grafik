﻿/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"

#include <memory>
#include <string>


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
    int width { 640 };
    int height { 480 };
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

    static Application& Get() { return *_application; }

    [[nodiscard]] Window* GetWindow() const { return _window.get(); }

private:
    void InitUI();
    static void CheckArgs(ApplicationConfig& config);
};
