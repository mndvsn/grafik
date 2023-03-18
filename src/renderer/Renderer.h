/**
 * Grafik
 * Renderer
 * Copyright Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"


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
    
    static bool IsRenderValid();

    // Temporary
    static void Render(const VertexArray& vao, const std::shared_ptr<Shader>& shader, int elementStart = 0, int elementEnd = 0);

    static void SetViewport(int width, int height);
    static bool GetFramebufferSize(int& width, int& height);
};
