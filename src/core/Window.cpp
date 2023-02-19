/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "Window.h"

#include "events/ApplicationEvent.h"

#include <GLFW/glfw3.h>


Window::Window(const WindowProperties& props)
    : _props { props }
{
    Init();
}

void Window::Init()
{
    glfwSetErrorCallback(glfwError);
    if (!glfwInit())
    {
        {
            throw std::runtime_error("GLFW initialization failed!");
        }
    }

    _context = GraphicsContext::Create();
    CreateNativeWindow();
    _context->Init(_window);

    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, const int width, const int height)
    {
        WindowProperties& props = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));
        props.width = width;
        props.height = height;

        WindowSizeEvent event(width, height);
        props.eventCallback(event);
    });
}

void Window::CreateNativeWindow()
{
    // Create window and init glfw with context
    _window = glfwCreateWindow(static_cast<int>(_props.width), static_cast<int>(_props.height), _props.title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }
    glfwSetWindowUserPointer(_window, &_props);
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
