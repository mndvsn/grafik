/**
* Grafik
 * Vulkan UI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "ui/UI.h"


class VulkanUI : public UI
{
public:
    ~VulkanUI() override;
    
    void Init(GLFWwindow* window) override;

    void Begin() override;
    void End() override;
};
