/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "Application.h"

#include "core/Window.h"
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

#include <imgui/imgui.h>

#include <iostream>


extern bool appShouldExit;

//#define DRAW_WIREFRAME

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

    WindowProperties props { _config.width, _config.height, _config.title };
    _window = std::make_unique<Window>(props);

    // Init ImGUI
    InitUI();
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
    std::cout << "Application::Run()" << std::endl;

#ifdef DRAW_WIREFRAME
    // Draw in wireframe mode
    RenderCommand::SetWireframeMode(true);
#endif

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

        if (_ui)
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

Application::~Application()
{
    _window->Shutdown();
}

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