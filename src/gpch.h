/**
 * Grafik
 * Copyright Martin Furuberg
 */
#pragma once

#include "core/Platform.h"

#include <iostream>
#include <functional>
#include <memory>
#include <numbers>
#include <stdexcept>

#include <array>
#include <string>
#include <sstream>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

#if !defined(NDEBUG) || defined(_DEBUG)
    #define GK_DEBUG
#endif

#ifdef GK_WIN
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

#include "core/Common.h"
