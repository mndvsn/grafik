/**
 * Grafik
 * OpenGLContext
 * Copyright 2023 Martin Furuberg 
 */
#include "OpenGLContext.h"

#ifdef _DEBUG
#include "utils/GLDebug.h"
#endif

#include <iostream>


OpenGLContext::OpenGLContext()
{
    // Set OpenGL context to 4.6 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
}

void OpenGLContext::Init(GLFWwindow* window)
{
    _window = window;
    
    glfwMakeContextCurrent(_window);

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

    // swap buffers in sync with screen freq aka v-sync
    glfwSwapInterval(1);

    // Set OpenGL state
    SetState();
}

void OpenGLContext::SetState()
{
    // Set blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(_window);
}

#ifdef _DEBUG
void OpenGLContext::InitDebug() const
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
