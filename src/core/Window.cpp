/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#include "Window.h"

#include "renderer/GraphicsContext.h"

#include <iostream>


void Window::Init(const std::string& title, const int width, const int height)
{
    glfwSetErrorCallback(glfwError);
    if (!glfwInit())
    {
        {
            throw std::runtime_error("GLFW initialization failed!");
        }
    }

    _context = GraphicsContext::Create();

    CreateWindow(title.c_str(), width, height);

    _context->Init(_window);
}

void Window::CreateWindow(const char* title, int width, int height)
{
    // Create window and init glfw with context
    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }
}

void Window::Update()
{
    _context->SwapBuffers();
    glfwPollEvents();
}

bool Window::IsRunning() const
{
    return !glfwWindowShouldClose(_window);
}

void Window::Shutdown()
{
    
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
