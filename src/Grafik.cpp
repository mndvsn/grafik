/**
 * Grafik
 * Grafik
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "GLRender.h"

#include <iostream>


int main()
{
    GLRender render("Grafik", 800, 600);

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
