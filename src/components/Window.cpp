/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "Window.h"

#include "events/ApplicationEvent.h"
#include "renderer/RendererAPI.h"

#include <GLFW/glfw3.h>


Window::Window(const WindowProperties& props)
    : _state { props.title, props.width, props.height } { }

void Window::OnAttach(int& eventMask)
{
    eventMask = Event::Category::None;

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
        WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
        state.SetSize(width, height);

        WindowSizeEvent event(width, height);
        EventManager::Get()->Broadcast(event);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow*)
    {
        WindowCloseEvent event;
        EventManager::Get()->Broadcast(event);
    });

    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow*, const int width, const int height)
    {
        FramebufferSizeEvent event(width, height);
        EventManager::Get()->Broadcast(event);
    });
}

void Window::CreateNativeWindow()
{
    // Create window and init glfw with context
    _window = glfwCreateWindow(static_cast<int>(_state.width), static_cast<int>(_state.height), GetDetailedWindowTitle().c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("Window context initialization failed!");
        }
    }
    _state.running = true;
    glfwSetWindowUserPointer(_window, &_state);
}

void Window::Update()
{
    glfwPollEvents();
    _context->SwapBuffers();
}

std::string Window::GetDetailedWindowTitle() const
{
    std::stringstream title;
    title << _state.title << " - " << RendererAPI::GetAPIString();
    return title.str();
}

void Window::Close()
{
    _state.running = false;
    _context->Shutdown();
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::glfwError(int error, const char* description)
{
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}
