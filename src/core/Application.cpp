/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "Application.h"
#include "GLWindow.h"
#include "VulkanWindow.h"
#include "Renderer.h"

// Labb
#include "labb/Batch.h"
#include "labb/ClearColor.h"
#include "labb/Mirror.h"
#include "labb/Loop.h"
#include "labb/Stacks.h"
#include "labb/Triangle.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <iostream>


static Application* application { nullptr };
extern bool appShouldExit;

//#define DRAW_WIREFRAME

Application::Application(ApplicationConfig config)
    : _config { std::move(config) }
{
    application = this;
    
    // Parse launch arguments
    CheckArgs(_config);
}

void Application::Init()
{
    //TODO: GLFW window wrappers for OpenGL + Vulkan
    if (_config.bVulkan)
    {
        window = std::make_unique<VulkanWindow>();
    }
    else
    {
        window = std::make_unique<GLWindow>();
    }
    
    window->Init(_config.title, _config.width, _config.height);
}

void Application::Run() const
{
    std::cout << "Application::Run()" << std::endl;

    // temp Vulkan bypass
    if (_config.bVulkan)
    {
        while (window->IsRunning())
        {
            window->BeginFrame();
            window->EndFrame();
        }
        appShouldExit = true;
        return;
    }
    
    Renderer renderer(window->GetWindow());
    
    double totalTimeElapsed { 0 },
        timeElapsedNow { 0 },
        deltaTime { 0 };

#ifdef DRAW_WIREFRAME
    // Draw in wireframe mode
    renderer.SetWireframeMode(true);
#endif

    // Set to false to exit active lab
    static bool bKeep { true };
    auto lab = std::unique_ptr<labb::LLab> {};
    const auto menu = std::make_unique<labb::LLabMenu>(renderer, lab);

    // Add labs to main menu
    menu->RegisterLab<labb::LClearColor>("Clear Color", "clearcolor");
    menu->RegisterLab<labb::LTriangle>("Triangle", "triangle");
    menu->RegisterLab<labb::LStacks>("Stacks", "stacks");
    menu->RegisterLab<labb::LMirror>("Mirror", "mirror");
    menu->RegisterLab<labb::LBatch>("Batch", "batch");
    menu->RegisterLab<labb::LLoop>("Loop", "loop");

    // Create an initial lab if requested and matching shortname is found
    if (!_config.initLab.empty())
    {
        if (const auto maybeLab = menu->CreateLabIfExists(_config.initLab))
        {
            lab.reset(*maybeLab);
        }
    }
    
    // Keep running until we should close and exit
    while (window->IsRunning())
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;

        // Reset context state
        window->BeginFrame();

        if (lab)
        {
            lab->BeginUpdate(deltaTime);
            lab->BeginRender();
        }
        else
        {
            menu->BeginRender();
        }

        BeginImGUI();

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
        RenderImGUI();
        
        window->EndFrame();
    }

    appShouldExit = true;
}

void Application::BeginImGUI() const
{
    // Begin ImGUI frame
    window->BeginImGUI();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();
}

void Application::RenderImGUI() const
{
    // Render ImGUI
    ImGui::Render();
    window->RenderImGUI();
}

Application& Application::Get()
{
    return *application;
}

Application::~Application()
{
    window->Shutdown();
}

void Application::CheckArgs(ApplicationConfig& config)
{
    for (int i = 0; i < config.args.count; i++)
    {
        // Look for vulkan flag
        if (strcmp(config.args[i], "-vulkan") == 0)
        {
            config.bVulkan = true;
        }
        // Look for lab init option
        else if (config.args.count > i+1 && strcmp(config.args[i], "-l") == 0)
        {
            config.initLab = config.args[i+1];
        }
    }
}