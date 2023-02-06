/**
 * Grafik
 * RenderApp
 * Copyright Martin Furuberg
 */
#include "RenderApp.h"
#include <iostream>


void RenderApp::glfwError(int error, const char* description)
{
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}

RenderApp::RenderApp(const char* title, int width, int height, const char* initLab)
    : _title { title }
    , _width { width }
    , _height { height }
    , _initLab { initLab }
{
    
}

RenderApp::~RenderApp()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
