/**
 * Grafik
 * Application
 * Copyright 2023 Martin Furuberg 
 */
#pragma once

#include <memory>
#include <string>


struct ApplicationArgs
{
    int count { 0 };
    char** values { nullptr };

    const char* operator[](const int index) const
    {
        return values[index];
    }
};

struct ApplicationConfig
{
    std::string title { };
    int width { 640 };
    int height { 480 };
    bool bVulkan { false };
    std::string initLab { };
    ApplicationArgs args { };
};

class IWindow;

class Application
{
    ApplicationConfig _config;
    std::unique_ptr<IWindow> window;
    
public:
    Application(ApplicationConfig config = ApplicationConfig());
    ~Application();
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static Application& Get();

    void Init();
    void Setup();
    void Run() const;
    
private:
    static void CheckArgs(ApplicationConfig& config);
    
};
