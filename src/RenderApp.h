/**
 * Grafik
 * RenderApp
 * Copyright Martin Furuberg 
 */
#pragma once
#include <GLFW/glfw3.h>

#include <string>

class RenderApp
{
protected:
    GLFWwindow* _window { nullptr };
    std::string _title { };
    int _width { 640 };
    int _height { 480 };
    std::string _initLab { };

public:
    RenderApp(const char* title, int width, int height, const char* initLab);
    virtual ~RenderApp();

    RenderApp(const RenderApp &) = delete;
    RenderApp &operator=(const RenderApp &) = delete;
    
    virtual void Init() = 0;
    virtual void Setup() = 0;
    virtual void Run() = 0;

protected:
    static void glfwError(int error, const char* description);

private:
    virtual void InitImGUI() = 0;
#ifdef _DEBUG
    virtual void InitDebug() = 0;
#endif
};
