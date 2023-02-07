/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "Application.h"
#include "GLWindow.h"
#include "Renderer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <iostream>


static Application* application { nullptr };
extern bool appShouldExit;

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
    if (!_config.bVulkan)
    {
        window = std::make_unique<GLWindow>();
    }
    
    if (!window) return;

    window->Init(_config.title, _config.width, _config.height);
}

void Application::Setup()
{
    
}

void Application::Run() const
{
    std::cout << "Application::Run()" << std::endl;

    Renderer renderer(window->GetWindow());

    // Keep running until we should close and exit
    while (window && window->IsRunning())
    {
        BeginImGUI();
        RenderImGUI();
        
        window->Loop();
    }

    appShouldExit = true;
}

void Application::BeginImGUI() const
{
    // Begin ImGUI frame
    window->BeginImGUI();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
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
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::CheckArgs(ApplicationConfig& config)
{
    for (int i = 0; i < config.args.count; i++)
    {
        // Look for vulkan flag
        if (strcmp(config.args[i], "-vulkan") == 0)
        {
            std::cout << "Using Vulkan" << std::endl;
            config.bVulkan = true;
        }
        // Look for lab init option
        else if (config.args.count > i+1 && strcmp(config.args[i], "-l") == 0)
        {
            config.initLab = config.args[i][i+1];
        }
    }
}