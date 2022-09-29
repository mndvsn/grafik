/**
 * Grafik
 * Grafik
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "GLRender.h"

#include <iostream>


const char* findInitialLab(int n, char* args[]);

int main(const int argc, char *argv[])
{
    const char* lab = findInitialLab(argc, argv);
    GLRender render("Grafik", 1100, 750, lab);

    try
    {
        render.Init();
        render.Setup();
    }
    catch (const std::runtime_error& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }
    
    render.Run();
    
    return 0;
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