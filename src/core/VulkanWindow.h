/**
 * Grafik
 * VulkanWindow
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#define GLFW_INCLUDE_VULKAN
#include "WindowBase.h"


class VulkanWindow : public Window, public IWindow
{
public:
    VulkanWindow() = default;
    ~VulkanWindow() override;

    void Init(const std::string& title, int width, int height) override;
    void Setup() override;
    void BeginFrame() override;
    void Update() override;

    void BeginImGUI() const override;
    void RenderImGUI() const override;

    [[nodiscard]] GLFWwindow* GetWindow() const override { return _window; }
    [[nodiscard]] bool IsRunning() const override;

    void Shutdown() override;

protected:
    void CreateWindow(const std::string& title, int width, int height) override;
    void InitImGUI() override;

#ifdef _DEBUG
    void InitDebug() override;
#endif
};
