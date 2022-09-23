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
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "utils/File.h"
#include "utils/GLDebug.h"


GLFWwindow* window { nullptr };

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check for errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length {};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader failed: ";
        std::cout << message << "\n";

        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    const unsigned int program = glCreateProgram();
    const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

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
    
    File vsFile("src/res/shaders/basic.vs");
    const std::string vertexShader = vsFile.Read();
    
    File fsFile("src/res/shaders/basic.fs");
    const std::string fragmentShader = fsFile.Read();
    
    const unsigned int shader = CreateShaderProgram(vertexShader, fragmentShader);
    glUseProgram(shader);

    int uniformLocation = glGetUniformLocation(shader, "u_Color");
    glUniform4f(uniformLocation, 1.0f, 0.0f, 1.0f, 1.0f); // set initial value

    // unbind state
    glUseProgram(0);
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Draw two triangles with created program, uniform and vertex array object
        glUseProgram(shader);

        cycle = fmod(cycle + 50*deltaTime, 360.0);
        auto diffSin = static_cast<float>(sin(cycle * std::numbers::pi / 180.0));
        auto diffCos = static_cast<float>(cos(cycle * std::numbers::pi / 180.0));
        glUniform4f(uniformLocation, 0.5f + diffCos*0.5f, 0.5f + diffSin*0.5f, 0.0f, 1.0f);

        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}
