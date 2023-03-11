/**
 * Grafik
 * Renderer
 * Copyright Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"

#include <glm/fwd.hpp>


class VertexArray;
class Shader;

class Renderer
{
public:
    Renderer() = default;

    static void Init(RendererAPI::API api);

    static void Render();
    
    static void BeginScene();
    static void EndScene();
    
    static void Render(const VertexArray& vao, const std::shared_ptr<Shader>& shader, int elementStart = 0, int elementEnd = 0);
    
    void Clear() const;
    void SetClearColor(const glm::vec3& color);
    void SetClearColor(const glm::vec4& color);
    
    void SetWireframeMode(bool bUseLineDraw);

    static void SetViewport(int width, int height);
    static bool GetFramebufferSize(int& width, int& height);
};
