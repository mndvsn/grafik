/**
 * Grafik
 * Grafik
 * Copyright Martin Furuberg
 */
#include "gpch.h"
#include "core/Application.h"


int grafik(const int argc, char** argv)
{
    Grafik::Init();

    while (!Grafik::ShouldExit)
    {
        Grafik::ShouldExit = false;
        
        const Application::Config config
        {
            .title   = Grafik::ApplicationName,
            .width   = Grafik::WindowWidth,
            .height  = Grafik::WindowHeight,
            .args    = { argc, argv }, 
        };
        const auto app = std::make_unique<Application>(config);

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
    }
    return EXIT_SUCCESS;
}

#if defined(GK_DISTR) && defined(GK_WIN)

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    return grafik(__argc, __argv);
}

#else

int main(const int argc, char** argv)
{
    return grafik(argc, argv);
}

#endif
