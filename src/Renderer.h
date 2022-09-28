/**
 * Grafik
 * Renderer
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>


class VertexArray;
class Shader;

class Renderer
{
    GLFWwindow* _context;

public:
    Renderer(GLFWwindow* window);
    ~Renderer();
    
    void Render(const VertexArray& vao, const Shader& shader, int elementStart = 0, int elementEnd = 0) const;

    void Clear() const;
    void SetClearColor(const glm::vec3& color);
    void SetClearColor(const glm::vec4& color);
    
    void SetWireframeMode(bool bUseLineDraw);

    bool GetFramebufferSize(int& width, int& height) const;

    [[nodiscard]] GLFWwindow* GetContext() const { return _context; }
};
