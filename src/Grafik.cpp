/**
 * Grafik 
 * Copyright 2012-2022 Martin Furuberg 
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static std::string LoadShader(const std::string& filePath)
{
    const std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

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
        int length;
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

int main(void)
{
    if (!glfwInit())
    {
        return 1;
    }
    
    // Create window and init glfw with context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Grafik", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        return 1;
    }

    // Print adapter info
    std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    std::cout << "Extensions:\n";
    for (GLint i = 0; i < numExtensions; i++)
    {
        std::cout << glGetStringi(GL_EXTENSIONS, i) << "\n";
    }

    // Data for triangle
    const float positions[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };

    // Generate buffer for static draw
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Define position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    const std::string vertexShader = LoadShader("src/res/shaders/basic.vs");
    const std::string fragmentShader = LoadShader("src/res/shaders/basic.fs");
    
    const unsigned int shader = CreateShaderProgram(vertexShader, fragmentShader);
    glUseProgram(shader);
    
    // Keep loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Clean up
    glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}
