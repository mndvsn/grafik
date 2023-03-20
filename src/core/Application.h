/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "core/ComponentManager.h"
#include "events/ApplicationEvent.h"
#include "events/KeyboardEvent.h"
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
    std::shared_ptr<UI> _ui { };
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
    template <typename T>
    [[nodiscard]] std::weak_ptr<T> GetUI() const { return std::dynamic_pointer_cast<T>(_ui); }

private:
    void InitUI();
    void InitLabs();

    void OnWindowClose(WindowCloseEvent& e) const;
    void OnInitLab(InitLabEvent& e);
    void OnKey(KeyEvent& e) const;
    
    static void CheckArgs(Config& config);
};
