/**
 * Grafik
 * OpenGLContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include "renderer/GraphicsContext.h"


struct GLFWwindow;

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext();

    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext& operator=(const OpenGLContext&) = delete;

    void Init(GLFWwindow* window) override;
    void SwapBuffers() override;

    void SetState();
    
    void Resize(unsigned width, unsigned height) override { _width = width; _height = height; }
    [[nodiscard]] std::pair<unsigned, unsigned> GetSize() const override { return { _width, _height }; }

    void Shutdown() override { }

#ifdef _DEBUG
    void InitDebug() const;
#endif
    
private:
    unsigned _width { 0 };
    unsigned _height { 0 };
};
