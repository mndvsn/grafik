/**
 * Grafik
 * Grafik
 * Copyright 2012-2022 Martin Furuberg 
 */
#ifdef GK_WIN
#include "core/Application.h"

#include <iostream>


bool appShouldExit { false };

int Grafik(const int argc, char** argv)
{
    while (!appShouldExit)
    {
        ApplicationConfig config("Grafik", 1100, 750);
        config.args = { argc, argv };

        const auto app = new Application(config);

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
    }
    return EXIT_SUCCESS;
}

#ifdef GK_DISTR

#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    return Grafik(__argc, __argv);
}

#else

int main(const int argc, char** argv)
{
    return Grafik(argc, argv);
}

#endif
#endif