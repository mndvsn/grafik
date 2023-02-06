/**
 * Grafik
 * Grafik
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "GLRender.h"
#include "VulkanRender.h"

#include <iostream>


const char* findInitialLab(int n, char* args[]);
bool checkVulkan(const int n, char* args[]);

int main(const int argc, char *argv[])
{
    RenderApp* app = nullptr;
    
    const bool bVulkan = checkVulkan(argc, argv);
    const char* lab = findInitialLab(argc, argv);

    //TODO: Implement a common APP class, and GLFW window wrappers for OpenGL + Vulkan
    if (bVulkan)
    {
        app = new VulkanRender("Grafik", 1100, 750, lab);
    }
    else
    {
        app = new GLRender("Grafik", 1100, 750, lab);
    }
    

    try
    {
        app->Init();
        app->Setup();
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    app->Run();

    delete app;
    
    return EXIT_SUCCESS;
}

const char* findInitialLab(const int n, char* args[])
{
    const char* finding { "" };
    for (int i=0; i<n; i++)
    {
        if (strcmp(args[i], "-l") == 0 && n > i)
        {
            finding = args[i+1];
        }
    }
    return finding;
}

bool checkVulkan(const int n, char* args[])
{
    for (int i=0; i<n; i++)
    {
        if (strcmp(args[i], "-vulkan") == 0)
        {
            std::cout << "Using Vulkan" << std::endl;
            return true;
        }
    }
    return false;
}
