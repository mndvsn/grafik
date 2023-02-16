/**
 * Grafik
 * VulkanContext
 * Copyright 2023 Martin Furuberg 
 */
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

#include <iostream>


VulkanContext::VulkanContext()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanContext::Init(GLFWwindow* window)
{
    _window = window;

#   ifdef _DEBUG
    InitDebug();
#   endif
}

void VulkanContext::SwapBuffers()
{
    
}

#ifdef _DEBUG
void VulkanContext::InitDebug() const
{
    // Print adapter info
    std::cout << "Vulkan" << "\n";
}
#endif
