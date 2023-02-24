﻿/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "Application.h"

#include "core/Window.h"
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "ui/UI.h"

// Labb
#include "labb/LabMenu.h"
#include "labb/Batch.h"
#include "labb/ClearColor.h"
#include "labb/Mirror.h"
#include "labb/Loop.h"
#include "labb/Stacks.h"
#include "labb/Triangle.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


extern bool appShouldExit;

Application::Application(ApplicationConfig config)
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
    EventManager::Get()->addListener(this, GK_BIND_EVENT_HANDLER(Application::OnEvent), Event::Category::Application);

    WindowProperties props { _config.title, _config.width, _config.height };
    _window = std::make_unique<Window>(props);
    _window->SetEventCallback(GK_BIND_EVENT_HANDLER(Application::OnEvent));

    // Init ImGUI
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

    if ((_ui = UI::Create()))
    {
        _ui->Init(_window->GetNativeWindow());
    }

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    IM_ASSERT(font != nullptr); (void)font;
}

void Application::InitLabs()
{
    _menu = std::make_shared<labb::LLabMenu>();

    if (_config.api == RendererAPI::API::OpenGL)
    {
        // Add labs to main menu
        _menu->RegisterLab<labb::LClearColor>("Clear Color", "clearcolor");
        _menu->RegisterLab<labb::LTriangle>("Triangle", "triangle");
        _menu->RegisterLab<labb::LStacks>("Stacks", "stacks");
        _menu->RegisterLab<labb::LMirror>("Mirror", "mirror");
        _menu->RegisterLab<labb::LBatch>("Batch", "batch");
        _menu->RegisterLab<labb::LLoop>("Loop", "loop");
    }
    // Add components to manager
    _components.Attach(_menu);

    // Create an initial lab if requested and matching shortname is found
    if (!_config.initLab.empty())
    {
        _menu->CreateLabIfExists(_config.initLab);
    }
}

void Application::Run()
{
    double totalTimeElapsed { 0 },
        timeElapsedNow { 0 },
        deltaTime { 0 };

    // Keep running until we should close and exit
    while (_window->IsRunning())
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;

        // Reset context state
        Renderer::BeginFrame();

        TickEvent tickEvent { deltaTime };
        EventManager::Get()->Broadcast(tickEvent);

        RenderEvent renderEvent;
        EventManager::Get()->Broadcast(renderEvent);

        if (_ui && !_window->IsMinimized())
        {
            _ui->Begin();

            UIEvent uiEvent;
            EventManager::Get()->Broadcast(uiEvent);

            // Render UI
            _ui->End();
        }

        Renderer::EndFrame();
        _window->Update();

        if (_components.Clean() && _components.GetCount() < 2)
        {
            _menu->ShowBigMenu();
        }
    }

    appShouldExit = true;
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher { e };
    dispatcher.Dispatch<WindowSizeEvent>(GK_BIND_EVENT_HANDLER(OnWindowResize));
    dispatcher.Dispatch<WindowCloseEvent>(GK_BIND_EVENT_HANDLER(OnWindowClose));
    dispatcher.Dispatch<InitLabEvent>(GK_BIND_EVENT_HANDLER(OnInitLab));
}

void Application::OnWindowClose(WindowCloseEvent& e) const
{
    _window->Close();
    e.Handled();
}

void Application::OnWindowResize(const WindowSizeEvent& e) const
{
    if (_window->IsMinimized()) return;

    //TODO: Fix perspective
    Renderer::SetViewport(static_cast<int>(e.GetWidth()), static_cast<int>(e.GetHeight()));
}

void Application::OnInitLab(InitLabEvent& e)
{
    if (const auto lab = e.createLab())
    {
        _components.Attach(lab);
        
        if (_components.GetCount() > 1)
        {
            _menu->HideBigMenu();
        }
    }
    e.Handled();
}

Application::~Application() = default;

void Application::CheckArgs(ApplicationConfig& config)
{
    for (int i = 0; i < config.args.count; i++)
    {
        // Look for vulkan flag
        if (strcmp(config.args[i], "-vulkan") == 0)
        {
            config.api = RendererAPI::API::Vulkan;
        }
        // Look for lab init option
        else if (config.args.count > i+1 && strcmp(config.args[i], "-l") == 0)
        {
            config.initLab = config.args[i+1];
        }
    }
}