/**
 * Grafik
 * Window
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/MouseEvent.h"
#include "renderer/Renderer.h"

#include <GLFW/glfw3.h>


Window::Window(const WindowProperties& props)
    : _state { props.title, props.width, props.height } { }

void Window::OnAttach(int& eventMask)
{
    eventMask = Event::Application;

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

        EventManager::Get()->Broadcast<WindowSizeEvent>(width, height);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow*)
    {
        EventManager::Get()->Broadcast<WindowCloseEvent>();
    });

    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow*, const int width, const int height)
    {
        EventManager::Get()->Broadcast<FramebufferSizeEvent>(width, height);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow*, const double x, const double y)
    {
        // convert from subpixel data (possibly) to integer
        auto mx = static_cast<uint16_t>(x);
        auto my = static_cast<uint16_t>(y);
        EventManager::Get()->Broadcast<MouseMoveEvent>(mx, my);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow*, const double dx, const double dy)
    {
        // convert from high precision data (ie touch input) to integer
        auto deltaX = static_cast<uint16_t>(dx);
        auto deltaY = static_cast<uint16_t>(dy);
        EventManager::Get()->Broadcast<MouseScrollEvent>(deltaX, deltaY);
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow*, int button, int action, int mods)
    {
        const bool pressed = action == GLFW_PRESS;
        (void)mods; //TODO: handle modifier keys
        
        EventManager::Get()->Broadcast<MouseButtonEvent>(button, pressed);
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

void Window::OnEvent(Event& e)
{
    EventDispatcher(e).Dispatch<FramebufferSizeEvent>(GK_BIND_EVENT_HANDLER(OnFramebufferSize));
}

void Window::OnFramebufferSize(const FramebufferSizeEvent& e)
{
    //TODO: Fix perspective
    Renderer::SetViewport(static_cast<int>(e.GetWidth()), static_cast<int>(e.GetHeight()));

    // Redraw
    _context->Resize(e.GetWidth(), e.GetHeight());
    Renderer::Render();
}

void Window::Update()
{
    glfwPollEvents();
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
}

Window::~Window()
{
    _context.reset();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::glfwError(int error, const char* description)
{
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}
