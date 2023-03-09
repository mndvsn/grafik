/**
 * Grafik
 * OpenGLContext
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "OpenGLContext.h"

#ifdef GK_DEBUG
#include "renderer/opengl/OpenGLDebug.h"
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>


OpenGLContext::OpenGLContext()
{
    // Set OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Grafik::OpenGLAPIMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Grafik::OpenGLAPIMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef GK_DEBUG
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

#   ifdef GK_DEBUG
    InitDebug();
#   endif

    // swap buffers in sync with screen freq aka v-sync
    glfwSwapInterval(1);

    // Set OpenGL state
    SetState();
}

void OpenGLContext::SetState()
{
    int width { 0 }, height { 0 };
    glfwGetFramebufferSize(_window, &width, &height);
    Resize(width, height);
    
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    
    // Set blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
}

void OpenGLContext::SwapBuffers()
{
    if (_width != 0 && _height != 0)
    {
        glfwSwapBuffers(_window);
    }
}

#ifdef GK_DEBUG
void OpenGLContext::InitDebug() const
{
    Log::Info("{:*^50}", " OpenGL ");
    // Print adapter info
    Log::Info("{:<20} {}", "Vendor:",           reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Log::Info("{:<20} {}", "Renderer:",         reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    Log::Info("{:<20} {}", "Shading language:", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    Log::Info("{:<20} {}", "GL Version:",       reinterpret_cast<const char*>(glGetString(GL_VERSION)));

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
