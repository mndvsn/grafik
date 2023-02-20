/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "Application.h"

#include "core/Window.h"
#include "events/ApplicationEvent.h"
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "ui/UI.h"

// Labb
#include "labb/Lab.h"
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

    WindowProperties props { _config.title, _config.width, _config.height };
    _window = std::make_unique<Window>(props);
    _window->SetEventCallback(GK_BIND_EVENT_HANDLER(OnEvent));

    // Init ImGUI
    InitUI();

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
        _ui->Init(_window->GetSysWindow());
    }

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    IM_ASSERT(font != nullptr); (void)font;
}

void Application::Run() const
{
    double totalTimeElapsed { 0 },
        timeElapsedNow { 0 },
        deltaTime { 0 };

    // Set to false to exit active lab
    static bool bKeep { true };
    auto lab = std::unique_ptr<labb::LLab> {};
    const auto menu = std::make_unique<labb::LLabMenu>(lab);

    if (_config.api == RendererAPI::API::OpenGL)
    {
        // Add labs to main menu
        menu->RegisterLab<labb::LClearColor>("Clear Color", "clearcolor");
        menu->RegisterLab<labb::LTriangle>("Triangle", "triangle");
        menu->RegisterLab<labb::LStacks>("Stacks", "stacks");
        menu->RegisterLab<labb::LMirror>("Mirror", "mirror");
        menu->RegisterLab<labb::LBatch>("Batch", "batch");
        menu->RegisterLab<labb::LLoop>("Loop", "loop");
    }

    // Create an initial lab if requested and matching shortname is found
    if (!_config.initLab.empty())
    {
        if (const auto maybeLab = menu->CreateLabIfExists(_config.initLab))
        {
            lab.reset(*maybeLab);
        }
    }
    
    // Keep running until we should close and exit
    while (_window->IsRunning())
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;

        // Reset context state
        Renderer::BeginFrame();

        if (lab)
        {
            lab->BeginUpdate(deltaTime);
            lab->BeginRender();
        }
        else
        {
            menu->BeginRender();
        }

        if (_ui && !_window->IsMinimized())
        {
            _ui->Begin();

            // Draw main menu UI
            menu->BeginGUI(&bKeep);

            if (lab)
            {
                // Draw lab specific UI
                lab->BeginGUI(&bKeep);
                if (!bKeep)
                {
                    lab.reset();
                    bKeep = true;
                }
            }
            else
            {
                // Draw selection window
                menu->BeginBigMenu();
            }

            // Render UI
            _ui->End();
        }

        Renderer::EndFrame();
        _window->Update();
    }

    appShouldExit = true;
}

void Application::OnEvent(Event& e) const
{
    EventDispatcher dispatcher { e };
    dispatcher.Dispatch<WindowCloseEvent>(GK_BIND_EVENT_HANDLER(OnWindowClose));
    dispatcher.Dispatch<WindowSizeEvent>(GK_BIND_EVENT_HANDLER(OnWindowResize));
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