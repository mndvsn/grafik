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


//#define DRAW_WIREFRAME

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
    std::cout << "GLWindow::InitImGUI" << std::endl;
}

void GLWindow::Loop()
{
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool GLWindow::IsRunning() const
{
    return !glfwWindowShouldClose(_window);
}

GLWindow::~GLWindow()
{
    
}

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