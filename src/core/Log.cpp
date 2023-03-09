/**
 * Grafik
 * Log
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "Log.h"
#ifdef GK_LOGGING

#include <spdlog/sinks/stdout_color_sinks.h>


std::shared_ptr<spdlog::logger> Log::_logger;

void Log::Init(Level level)
{
    spdlog::set_pattern("%^[%R:%S:%e] %v%$");

    _logger = spdlog::stdout_color_mt("Grafik");
    _logger->set_level(static_cast<spdlog::level::level_enum>(level));
}
#endif
