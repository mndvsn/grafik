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

};
