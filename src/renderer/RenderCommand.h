/**
 * Grafik
 * RenderCommand
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"

#include <glm/fwd.hpp>

#include <memory>


class RenderCommand
{
public:
    static void Init(RendererAPI::API api);

    static void ResetState();
    
    static void ClearBuffer();

    static void SetClearColor(const glm::vec3& color);
    static void SetClearColor(const glm::vec4& color);
    static void SetWireframeMode(bool bUseLineDraw = true);
    
private:
    inline static std::unique_ptr<RendererAPI> _renderAPI { nullptr };
};
