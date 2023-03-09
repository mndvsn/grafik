/**
 * Grafik
 * Log (Suppressed)
 * Copyright 2023 Martin Furuberg
 */
#pragma once


class Log
{
public:
    enum class Level : int
    {
        All         = 0,
        Debug       = 1,
        Info        = 2,
        Warn        = 3,
        Err         = 4,
        Critical    = 5,
        Off         = 6
    };

    Log() = default;

    static void Init(...) {}

    template <typename... Args>
    static void Msg(Args...);
    template <typename... Args>
    static void Info(Args...);
    template <typename... Args>
    static void Debug(Args...);
    template <typename... Args>
    static void Warn(Args...);
    template <typename... Args>
    static void Error(Args...);
    template <typename... Args>
    static void Crit(Args...);
};

template <typename... Args>
void Log::Msg(Args...) {}
template <typename... Args>
void Log::Debug(Args...) {}
template <typename... Args>
void Log::Info(Args...) {}
template <typename... Args>
void Log::Warn(Args...) {}
template <typename... Args>
void Log::Error(Args...) {}
template <typename... Args>
void Log::Crit(Args...) {}
