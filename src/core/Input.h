/**
 * Grafik
 * Input
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include <spdlog/fmt/ostr.h>


using KeyID = uint16_t;

namespace Input
{
    enum class InputState : uint8_t
    {
        Release  = 0,
        Press    = 1,
        Repeat   = 2,
    };
    
    namespace Mouse
    {
        enum Button : KeyID
        {
            M1              = 0,
            M2              = 1,
            M3              = 2,
            M4              = 3,
            M5              = 4,
            M6              = 5,
            M7              = 6,
            M8              = 7,
            Left            = M1,
            Right           = M2,
            Middle          = M3,
            Last            = M8,
        };

        inline const char* BtnString(const Button b)
        {
            switch (b)
            {
                case Left:       return "Left";
                case Right:      return "Right";
                case Middle:     return "Middle";
                case M4:         return "Mouse4";
                case M5:         return "Mouse5";
                case M6:         return "Mouse6";
                case M7:         return "Mouse7";
                case M8:         return "Mouse8";
            }
            return "Unknown";
        }
        
        inline std::ostream& operator<<(std::ostream& os, const Button& b)
        {
            return os << BtnString(b);
        }
    }
}
