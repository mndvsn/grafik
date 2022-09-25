/**
 * Grafik
 * GLRender
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>

#include <optional>
#include <string>


class GLRender
{
private:
    GLFWwindow* _window { nullptr };
    std::string _title { };
    int _width { 640 };
    int _height { 480 };

public:
    GLRender(const char* title, const int width, const int height);
    ~GLRender();

    void Init();
    void Setup();
    void Run();

    std::optional<VertexArray> vao {};
    std::optional<Shader> basicShader {};
    std::optional<Texture> texture {};

    // Matrices
    glm::mat4 projection {};
    glm::mat4 view {};
    glm::mat4 model {};
    glm::mat4 mvp {};
    
private:
    void InitImGUI();
    static void glfwError(int error, const char* description);
#ifdef _DEBUG
    static void InitDebug();
#endif
};
