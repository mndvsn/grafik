/**
 * Grafik
 * Grafik
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "core/Application.h"


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
            app->Run();
        }
        catch (const std::runtime_error& ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }

        delete app;
    }
    return EXIT_SUCCESS;
}

#ifdef GK_DISTR

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
