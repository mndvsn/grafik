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
#include <unordered_map>
#include <vector>

#ifdef GK_WIN
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

#include "core/Common.h"
