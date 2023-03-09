/**
 * Grafik
 * Log
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#ifdef GK_LOGGING
#include <spdlog/spdlog.h>


class Log
{
public:
    enum class Level : int
    {
        All         = SPDLOG_LEVEL_TRACE,
        Debug       = SPDLOG_LEVEL_DEBUG,
        Info        = SPDLOG_LEVEL_INFO,
        Warn        = SPDLOG_LEVEL_WARN,
        Err         = SPDLOG_LEVEL_ERROR,
        Critical    = SPDLOG_LEVEL_CRITICAL,
        Off         = SPDLOG_LEVEL_OFF
    };

    Log() = default;

    static void Init(Level level = Level::All);

    static std::shared_ptr<spdlog::logger> Get() { return _logger; }

    template <typename... Args>
    static void Msg(spdlog::format_string_t<Args...> fmt, Args... args);

    template <typename... Args>
    static void Debug(spdlog::format_string_t<Args...> fmt, Args... args);

    template <typename... Args>
    static void Info(spdlog::format_string_t<Args...> fmt, Args... args);

    template <typename... Args>
    static void Warn(spdlog::format_string_t<Args...> fmt, Args... args);

    template <typename... Args>
    static void Error(spdlog::format_string_t<Args...> fmt, Args... args);

    template <typename... Args>
    static void Crit(spdlog::format_string_t<Args...> fmt, Args... args);

private:
    static std::shared_ptr<spdlog::logger> _logger;
};

// Message/Trace
template <typename... Args>
void Log::Msg(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->trace(fmt, std::forward<Args>(args)...); }

// Debug
template <typename... Args>
void Log::Debug(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->debug(fmt, std::forward<Args>(args)...); }

// Info
template <typename... Args>
void Log::Info(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->info(fmt, std::forward<Args>(args)...); }

// Warning
template <typename... Args>
void Log::Warn(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->warn(fmt, std::forward<Args>(args)...); }

// Error
template <typename... Args>
void Log::Error(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->error(fmt, std::forward<Args>(args)...); }

// Critical
template <typename... Args>
void Log::Crit(spdlog::format_string_t<Args...> fmt, Args... args) { Get()->critical(fmt, std::forward<Args>(args)...); }

#else
#include "LogN.h"
#endif
