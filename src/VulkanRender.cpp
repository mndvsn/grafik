/**
 * Grafik
 * VulkanRender
 * Copyright Martin Furuberg 
 */
#include "VulkanRender.h"

#include "Pipeline.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#include <iostream>


VulkanRender::VulkanRender(const char* title, const int width, const int height, const char* initLab)
    : RenderApp(title, width, height, initLab)
{
    
}

void VulkanRender::Init()
{
    glfwSetErrorCallback(RenderApp::glfwError);
    if (!glfwInit())
    {
        {
            throw std::runtime_error("GLFW initialization failed!");
        }
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window and init glfw with context
    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }

#   ifdef _DEBUG
    InitDebug();
#   endif
}

void VulkanRender::InitImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark(&style);
    style.WindowRounding = 3.0f;
    style.FrameRounding = 3.0f;

    // Setup GLFW + Vulkan
    ImGui_ImplGlfw_InitForVulkan(_window, true);
    // ImGui_ImplVulkan_Init( ImGui_ImplVulkan_InitInfo );

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    IM_ASSERT(font != nullptr); (void)font;
}

void VulkanRender::Setup()
{
    Pipeline vulkanPipelineTest("data/shaders/vktest.vert.spv", "data/shaders/vktest.frag.spv");
    
}

void VulkanRender::Run()
{
    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

#ifdef _DEBUG
void VulkanRender::InitDebug()
{
    // Print adapter info
    
}
#endif
