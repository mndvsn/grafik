/**
 * Grafik
 * VulkanWindow
 * Copyright 2023 Martin Furuberg 
 */
#include "VulkanWindow.h"

#include "Pipeline.h"

#include <iostream>
#include <sstream>


void VulkanWindow::Init(const std::string& title, int width, int height)
{
    Window::InitGLFW();

    CreateWindow(title, width, height);

#   ifdef _DEBUG
    InitDebug();
#   endif

    // Setup Dear ImGui context
    InitImGUI();

    // Configure state
    //Setup();
}

void VulkanWindow::CreateWindow(const std::string& title, int width, int height)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window and init glfw with context
    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }
}

void VulkanWindow::InitImGUI()
{
}

void VulkanWindow::Setup()
{
    // Make pipeline
    Pipeline vulkanPipelineTest("data/shaders/vktest.vert.spv", "data/shaders/vktest.frag.spv");
}

void VulkanWindow::BeginFrame()
{
}

void VulkanWindow::EndFrame()
{
    glfwPollEvents();
}

void VulkanWindow::BeginImGUI() const
{
}

void VulkanWindow::RenderImGUI() const
{
}

bool VulkanWindow::IsRunning() const
{
    return !glfwWindowShouldClose(_window);
}

void VulkanWindow::Shutdown()
{
}

VulkanWindow::~VulkanWindow() = default;

#ifdef _DEBUG
void VulkanWindow::InitDebug()
{
    // Print adapter info
    std::cout << "Vulkan" << "\n";
}
#endif
