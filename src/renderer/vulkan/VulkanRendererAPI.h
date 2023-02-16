/**
 * Grafik
 * Vulkan RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/RendererAPI.h"


class VulkanRendererAPI : public RendererAPI
{
public:
    void ResetState() const override;
    
    void ClearBuffer() const override;
    
    void SetClearColor(float r, float g, float b, float alpha = 1.0f) override;
    void SetWireframeMode(bool bUseLineDraw) override;
};