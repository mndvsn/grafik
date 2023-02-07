/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg
 */
#include "Application.h"

#include <iostream>


static Application* application { nullptr };
extern bool appShouldExit;

Application::Application(ApplicationConfig config)
    : _config { std::move(config) }
{
    application = this;
    
    // Parse launch arguments
    CheckArgs(config);
}

void Application::Init()
{
    //TODO: GLFW window wrappers for OpenGL + Vulkan
}

void Application::Setup()
{
    
}

void Application::Run() const
{
    std::cout << "Application::Run()" << std::endl;

    appShouldExit = true;
}

Application& Application::Get()
{
    return *application;
}

Application::~Application() = default;

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