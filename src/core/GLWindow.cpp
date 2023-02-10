/**
 * Grafik
 * GLWindow
 * Copyright 2023 Martin Furuberg 
 */
#include "GLWindow.h"
#ifdef _DEBUG
#include "utils/GLDebug.h"
#endif

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <iostream>
#include <sstream>


void GLWindow::Init(const std::string& title, const int width, const int height)
{
    Window::InitGLFW();

    CreateWindow(title, width, height);

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        {
            throw std::runtime_error("GLAD initialization failed!");
        }
    }

#   ifdef _DEBUG
    InitDebug();
#   endif

    // Setup Dear ImGui context
    InitImGUI();

    // Set context state
    Setup();
}

void GLWindow::CreateWindow(const std::string& title, const int width, const int height)
{
    // Set OpenGL context to 4.6 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create window and init glfw with context
    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }
    glfwMakeContextCurrent(_window);

    // swap buffers in sync with screen freq aka v-sync
    glfwSwapInterval(1);
}

void GLWindow::InitImGUI()
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

    // Setup GLFW + OpenGL + GLSL
    std::stringstream glsl_version;
    glsl_version << "#version " << glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    IM_ASSERT(font != nullptr); (void)font;
}

void GLWindow::Setup()
{
    // Set blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWindow::BeginFrame()
{
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

void GLWindow::EndFrame()
{
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void GLWindow::BeginImGUI() const
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    
    ImGui_ImplOpenGL3_NewFrame();
}

void GLWindow::RenderImGUI() const
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GLWindow::IsRunning() const
{
    return !glfwWindowShouldClose(_window);
}

void GLWindow::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    
    //
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLWindow::~GLWindow() = default;

#ifdef _DEBUG
void GLWindow::InitDebug()
{
    // Print adapter info
    std::cout << "Shading language: \t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "GL Version: \t\t"     << glGetString(GL_VERSION) << "\n";
    std::cout << "Vendor: \t\t"         << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: \t\t"       << glGetString(GL_RENDERER) << std::endl;

    // Set up debug/error message handling
    int flags {};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(HandleGLDebugMessage, nullptr);
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
    }
}
#endif