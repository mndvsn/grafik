/**
 * Grafik
 * VulkanContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"


class VulkanContext : public GraphicsContext
{
public:
    VulkanContext();

    void Init(GLFWwindow* window) override;
    void SwapBuffers() override;

#ifdef _DEBUG
    void InitDebug() const;
#endif
};
