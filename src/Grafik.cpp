/**
 * Grafik 
 * Copyright 2012-2022 Martin Furuberg 
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>

#include "ElementBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "utils/GLDebug.h"


GLFWwindow* window { nullptr };

int InitOGL(const char* title, const int width, const int height)
{
    if (!glfwInit())
    {
        return 1;
    }

    // Set context to 4.6 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create window and init glfw with context
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // swap buffers in sync with screen freq aka v-sync
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        return 1;
    }

#ifdef _DEBUG
    // Print adapter info
    std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

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
#endif
    
    return 0;
}

int main()
{
    if (int ret = InitOGL("Grafik", 600, 600); ret != 0)
    {
        std::cout << "OpenGL initialization failed!" << std::endl;
        return ret;
    }

    double totalTimeElapsed { 0 }, timeElapsedNow { 0 }, deltaTime { 0 };
    
    // Data for triangle
    constexpr float positions[] =
    {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
       -0.5f,  0.5f
    };

    constexpr unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // Generate Vertex Array Object
    VertexArray vao;
        
    // Generate vertex buffer for static draw
    VertexBuffer vBuffer(positions, sizeof(positions));

    // Define layout
    VertexBufferLayout layout;
    layout.Push<float>(2); // position attribute, 2 floats

    // Add buffer with attributes to VAO
    vao.AddBuffer(vBuffer, layout);

    // Bind element/index buffer
    ElementBuffer eBuffer(indices, 6);

    // Create basic shader
    Shader basicShader("src/res/shaders/basic.vert", "src/res/shaders/basic.frag");
    basicShader.Bind();
    basicShader.SetUniform4f("u_Color", 1.0f, 0.0f, 1.0f, 1.0f); // set initial color

    // unbind state
    Shader::Unbind();
    VertexArray::Unbind();
    VertexBuffer::Unbind();
    
    double cycle {};
    
    // Keep loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;
        
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw in wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw two triangles with created program, uniform and vertex array object
        basicShader.Bind();

        cycle = fmod(cycle + 50*deltaTime, 360.0);
        const auto diffSin = static_cast<float>(sin(cycle * std::numbers::pi / 180.0));
        const auto diffCos = static_cast<float>(cos(cycle * std::numbers::pi / 180.0));
        basicShader.SetUniform4f("u_Color", 0.5f + diffCos*0.5f, 0.5f + diffSin*0.5f, 0.0f, 1.0f);

        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
