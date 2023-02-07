/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#include "WindowBase.h"

#include <iostream>


void Window::InitGLFW()
{
    glfwSetErrorCallback(Window::glfwError);
    if (!glfwInit())
    {
        {
            throw std::runtime_error("GLFW initialization failed!");
        }
    }
}

void Window::glfwError(int error, const char* description)
{
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
