/**
 * Grafik
 * GLRender
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


class GLRender
{
    GLFWwindow* _window { nullptr };
    std::string _title { };
    int _width { 640 };
    int _height { 480 };
    bool _bVulkan { false };
    std::string _initLab { };

public:
    GLRender(const char* title, int width, int height, const bool useVulkan, const char* initLab);
    ~GLRender();

    void Init();
    void Setup();
    void Run();
    void TempRun();

private:
    void InitImGUI();
    static void glfwError(int error, const char* description);
#ifdef _DEBUG
    static void InitDebug();
#endif
};
