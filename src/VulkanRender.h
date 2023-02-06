/**
 * Grafik
 * VulkanRender
 * Copyright Martin Furuberg 
 */
#pragma once
#define GLFW_INCLUDE_VULKAN
#include "RenderApp.h"

class VulkanRender : public RenderApp
{

public:
    VulkanRender(const char* title, int width, int height, const char* initLab);
    // ~VulkanRender() override;

    void Init() override;
    void Setup() override;
    void Run() override;

private:
    void InitImGUI() override;
#ifdef _DEBUG
    void InitDebug() override;
#endif
};
