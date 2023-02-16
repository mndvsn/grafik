/**
 * Platform macros
 * Copyright Martin Furuberg
 */
#pragma once

// Windows
#ifdef _WIN32
    // x64
    #ifdef _WIN64
        #define GK_WIN
    // x86
    #else
        #error "Windows x86 is not supported!"
    #endif

// Apple (macOS, iOS, iPadOS, ...)
#elif defined(__APPLE__) || defined(__MACH__)
    #define GK_MACOS
    #error "MacOS is not supported!"

// Linux
#elif defined(__linux__)
    #define GK_LINUX
    #error "Linux is not supported!"

// Unknown
#else
    #error "Unknown platform!"
#endif
