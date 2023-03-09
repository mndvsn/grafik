/**
 * Grafik
 * Common
 * Copyright 2023 Martin Furuberg
 */
#pragma once


namespace Grafik
{
    constexpr const char*       ApplicationName         { "Grafik" };
    constexpr const char*       EngineName              { "Engine" };
    constexpr unsigned          WindowWidth             { 1100 };
    constexpr unsigned          WindowHeight            { 750 };
    
    constexpr unsigned          OpenGLAPIMajor          { 4 };
    constexpr unsigned          OpenGLAPIMinor          { 6 };

    inline bool                 ShouldExit              { false };
    inline unsigned             APIOverride             { 0 };

    constexpr Log::Level        LoggingLevel            { Log::Level::All };


    inline void Init()
    {
        Log::Init(LoggingLevel);
    }
}

#define BIT(x) (1 << x)
