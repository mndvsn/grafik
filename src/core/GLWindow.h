/**
 * Grafik
 * GLWindow
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#include "WindowBase.h"


class GLWindow : public Window, public IWindow
{
public:
    GLWindow() = default;
    ~GLWindow() override;

    void Init(const std::string& title, int width, int height) override;
    void Loop() override;

    [[nodiscard]] bool IsRunning() const override;

protected:
    void CreateWindow(const std::string& title, int width, int height) override;
    void InitImGUI() override;

#ifdef _DEBUG
    void InitDebug() override;
#endif
};
