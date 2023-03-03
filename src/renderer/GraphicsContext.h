/**
 * Grafik
 * GraphicsContext
 * Copyright 2023 Martin Furuberg 
 */
#pragma once


struct GLFWwindow;

class GraphicsContext
{
public:
    GraphicsContext() = default;
    virtual ~GraphicsContext() = default;

    GraphicsContext(const GraphicsContext&) = delete;
    GraphicsContext& operator=(const GraphicsContext&) = delete;
    GraphicsContext(GraphicsContext&&) = delete;
    GraphicsContext& operator=(GraphicsContext&&) = delete;
    
    virtual void Init(GLFWwindow* window) = 0;
    virtual void SwapBuffers() = 0;
    
    virtual void Shutdown() = 0;

    virtual void Resize(unsigned width, unsigned height) = 0;
    [[nodiscard]] virtual std::pair<unsigned, unsigned> GetSize() const = 0;

    static std::unique_ptr<GraphicsContext> Create();

protected:
    GLFWwindow* _window { nullptr };
};
