/**
 * Grafik
 * Renderer
 * Copyright Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"

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

    static void Init(RendererAPI::API api);

    static void BeginFrame();
    static void EndFrame();
    
    static void BeginScene();
    static void EndScene();
    
    void Clear() const;
    void SetClearColor(const glm::vec3& color);
    void SetClearColor(const glm::vec4& color);
    
    void SetWireframeMode(bool bUseLineDraw);

    bool GetFramebufferSize(int& width, int& height) const;

    [[nodiscard]] GLFWwindow* GetContext() const { return _context; }
};
