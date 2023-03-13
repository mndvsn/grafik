/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "Application.h"

#include "components/Window.h"
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"

// Labb
#include "labb/LabMenu.h"
#include "labb/Batch.h"
#include "labb/ClearColor.h"
#include "labb/Mirror.h"
#include "labb/Loop.h"
#include "labb/Stacks.h"
#include "labb/Triangle.h"
#include "labb/VulkanTest.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


Application::Application(Config config)
    : _config { std::move(config) }
{
    _application = this;
    
    // Parse launch arguments
    CheckArgs(_config);
}

void Application::Init()
{
    Renderer::Init(_config.api);

    // Initialize event system
    EventManager::Get()->AddListener(this, GK_BIND_EVENT_HANDLER(Application::OnEvent), Event::Application);

    const WindowProperties props { _config.title, _config.width, _config.height };
    _window = _components.Create<Window>(props);

    InitUI();
    InitLabs();

    if (_config.wireFrameMode) RenderCommand::SetWireframeMode();
}

void Application::InitUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Configure style
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark(&style);
    style.WindowRounding = 3.0f;
    style.FrameRounding = 3.0f;

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    GK_ASSERT(font, "Font loading failed")
    io.FontDefault = font;

    if (((_ui = UI::Create())) && _window)
    {
        _ui->Init(_window->GetNativeWindow());
    }
}

void Application::InitLabs()
{
    // Add menu to component manager
    _menu = _components.Create<labb::LLabMenu>();

    // Add labs to main menu
    if (_config.api == RendererAPI::API::OpenGL)
    {
        _menu->RegisterLab<labb::LClearColor>("Clear Color", "clearcolor");
        _menu->RegisterLab<labb::LTriangle>("Triangle", "triangle");
        _menu->RegisterLab<labb::LStacks>("Stacks", "stacks");
        _menu->RegisterLab<labb::LMirror>("Mirror", "mirror");
        _menu->RegisterLab<labb::LBatch>("Batch", "batch");
        _menu->RegisterLab<labb::LLoop>("Loop", "loop");
    }
    else
    {
        _menu->RegisterLab<labb::LVulkanTest>("Vulkan", "vulkan");
    }

    // Create an initial lab if set to matching shortname
    if (!_config.initLab.empty())
    {
        _menu->CreateLabIfExists(_config.initLab);
    }
}

void Application::Run()
{
    double totalTimeElapsed { 0 };

    // Keep running until window is closed
    while (_window->IsRunning())
    {
        // Update timers
        const double timeElapsedNow = glfwGetTime();
        const double deltaTime      = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed            = timeElapsedNow;

        // Tick
        EventManager::Get()->Broadcast<TickEvent>(deltaTime);
        _window->Update();
        
        if (!_window->IsMinimized())
        {
            Renderer::Render();
        }

        if (_components.Clean() && _components.GetCount() < 3)
        {
            _menu->ShowBigMenu();
        }
    }
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher { e };
    dispatcher.Dispatch<WindowCloseEvent>(GK_BIND_EVENT_HANDLER(OnWindowClose));
    dispatcher.Dispatch<InitLabEvent>(GK_BIND_EVENT_HANDLER(OnInitLab));
}

void Application::OnWindowClose(WindowCloseEvent& e) const
{
    _window->Close();
    if (e.ShouldExit())
    {
        Grafik::ShouldExit = true;
    }
    e.Handled();
}

void Application::OnInitLab(InitLabEvent& e)
{
    if (const auto lab = e.createLab())
    {
        _components.Attach(lab);
        
        if (_components.GetCount() > 2)
        {
            _menu->HideBigMenu();
        }
    }
    e.Handled();
}

void Application::CheckArgs(Config& config)
{
    for (int i = 0; i < config.args.count; i++)
    {
        // Look for lab init option
        if (config.args.count > i+1 && (strcmp(config.args[i], "-lab") == 0 || strcmp(config.args[i], "-l") == 0))
        {
            config.initLab = config.args[i+1];
        }

        // Override Rendering API
        if (Grafik::APIOverride > 0)
        {
            config.api = static_cast<RendererAPI::API>(Grafik::APIOverride);
            break;
        }

        // Look for vulkan flag
        if (strcmp(config.args[i], "-vulkan") == 0 || strcmp(config.args[i], "-v") == 0)
        {
            config.api = RendererAPI::API::Vulkan;
        }
    }
}

Application::~Application()
{
    EventManager::Get()->Reset();
}
