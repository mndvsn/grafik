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

    namespace Keyboard
    {
        enum Key : KeyID
        {
            Space               = 32,
            Apostrophe          = 39, // '
            Comma               = 44, // ,
            Minus               = 45, // -
            Period              = 46, // .
            Slash               = 47, // /

            D0                  = 48, // 0
            D1                  = 49, // 1
            D2                  = 50, // 2
            D3                  = 51, // 3
            D4                  = 52, // 4
            D5                  = 53, // 5
            D6                  = 54, // 6
            D7                  = 55, // 7
            D8                  = 56, // 8
            D9                  = 57, // 9

            Semicolon           = 59, // ;
            Equal               = 61, // =

            A                   = 65,
            B                   = 66,
            C                   = 67,
            D                   = 68,
            E                   = 69,
            F                   = 70,
            G                   = 71,
            H                   = 72,
            I                   = 73,
            J                   = 74,
            K                   = 75,
            L                   = 76,
            M                   = 77,
            N                   = 78,
            O                   = 79,
            P                   = 80,
            Q                   = 81,
            R                   = 82,
            S                   = 83,
            T                   = 84,
            U                   = 85,
            V                   = 86,
            W                   = 87,
            X                   = 88,
            Y                   = 89,
            Z                   = 90,

            LeftBracket         = 91,  // [
            BackSlash           = 92,  /* \ */
            RightBracket        = 93,  // ]
            GraveAccent         = 96,  // `

            World1              = 161, // ISO 1
            World2              = 162, // ISO 2

            Escape              = 256,
            Enter               = 257,
            Tab                 = 258,
            Backspace           = 259,
            Insert              = 260,
            Delete              = 261,
            Right               = 262,
            Left                = 263,
            Down                = 264,
            Up                  = 265,
            PageUp              = 266,
            PageDown            = 267,
            Home                = 268,
            End                 = 269,
            CapsLock            = 280,
            ScrollLock          = 281,
            NumLock             = 282,
            PrintScreen         = 283,
            Pause               = 284,
            F1                  = 290,
            F2                  = 291,
            F3                  = 292,
            F4                  = 293,
            F5                  = 294,
            F6                  = 295,
            F7                  = 296,
            F8                  = 297,
            F9                  = 298,
            F10                 = 299,
            F11                 = 300,
            F12                 = 301,
            F13                 = 302,
            F14                 = 303,
            F15                 = 304,
            F16                 = 305,
            F17                 = 306,
            F18                 = 307,
            F19                 = 308,
            F20                 = 309,
            F21                 = 310,
            F22                 = 311,
            F23                 = 312,
            F24                 = 313,
            F25                 = 314,

            Num0                 = 320,
            Num1                 = 321,
            Num2                 = 322,
            Num3                 = 323,
            Num4                 = 324,
            Num5                 = 325,
            Num6                 = 326,
            Num7                 = 327,
            Num8                 = 328,
            Num9                 = 329,
            NumDecimal           = 330,
            NumDivide            = 331,
            NumMultiply          = 332,
            NumSubtract          = 333,
            NumAdd               = 334,
            NumEnter             = 335,
            NumEqual             = 336,

            // Special
            LeftShift           = 340,
            LeftCtrl            = 341,
            LeftAlt             = 342,
            LeftSuper           = 343,
            RightShift          = 344,
            RightCtrl           = 345,
            RightAlt            = 346,
            RightSuper          = 347,
            Menu                = 348
        };

        inline const char* KeyString(const Key k)
        {
            switch (k)
            {
                case Space:         return "Space";
                case Apostrophe:    return "Apostrophe";
                case Comma:         return "Comma";
                case Minus:         return "Minus";
                case Period:        return "Period";
                case Slash:         return "Slash";
                case Semicolon:     return "Semicolon";
                case Equal:         return "Equal";
                case LeftBracket:   return "LeftBracket";
                case BackSlash:     return "BackSlash";
                case RightBracket:  return "RightBracket";
                case GraveAccent:   return "GraveAccent";
                case World1:        return "World1";
                case World2:        return "World2";
                
                case D0:            return "Zero";
                case D1:            return "One";
                case D2:            return "Two";
                case D3:            return "Three";
                case D4:            return "Four";
                case D5:            return "Five";
                case D6:            return "Six";
                case D7:            return "Seven";
                case D8:            return "Eight";
                case D9:            return "Nine";                

                case A:             return "A";
                case B:             return "B";
                case C:             return "C";
                case D:             return "D";
                case E:             return "E";
                case F:             return "F";
                case G:             return "G";
                case H:             return "H";
                case I:             return "I";
                case J:             return "J";
                case K:             return "K";
                case L:             return "L";
                case M:             return "M";
                case N:             return "N";
                case O:             return "O";
                case P:             return "P";
                case Q:             return "Q";
                case R:             return "R";
                case S:             return "S";
                case T:             return "T";
                case U:             return "U";
                case V:             return "V";
                case W:             return "W";
                case X:             return "X";
                case Y:             return "Y";
                case Z:             return "Z";

                case Escape:        return "Esc";
                case Enter:         return "Enter";
                case Tab:           return "Tab";
                case Backspace:     return "Backspace";
                case Insert:        return "Insert";
                case Delete:        return "Del";
                case Right:         return "Right";
                case Left:          return "Left";
                case Down:          return "Down";
                case Up:            return "Up";
                case PageUp:        return "PageUp";
                case PageDown:      return "PageDown";
                case Home:          return "Home";
                case End:           return "End";
                case CapsLock:      return "CapsLock";
                case ScrollLock:    return "ScrollLock";
                case NumLock:       return "NumLock";
                case PrintScreen:   return "PrintScreen";
                case Pause:         return "Pause";
                case F1:            return "F1";
                case F2:            return "F2";
                case F3:            return "F3";
                case F4:            return "F4";
                case F5:            return "F5";
                case F6:            return "F6";
                case F7:            return "F7";
                case F8:            return "F8";
                case F9:            return "F9";
                case F10:           return "F10";
                case F11:           return "F11";
                case F12:           return "F12";
                case F13:           return "F12";
                case F14:           return "F12";
                case F15:           return "F12";
                case F16:           return "F12";
                case F17:           return "F12";
                case F18:           return "F12";
                case F19:           return "F12";
                case F20:           return "F12";
                case F21:           return "F12";
                case F22:           return "F12";
                case F23:           return "F12";
                case F24:           return "F12";
                case F25:           return "F12";

                case Num0:           return "Num0";
                case Num1:           return "Num1";
                case Num2:           return "Num2";
                case Num3:           return "Num3";
                case Num4:           return "Num4";
                case Num5:           return "Num5";
                case Num6:           return "Num6";
                case Num7:           return "Num7";
                case Num8:           return "Num8";
                case Num9:           return "Num9";
                case NumDecimal:     return "NumDecimal";
                case NumDivide:      return "NumDivide";
                case NumMultiply:    return "NumMultiply";
                case NumSubtract:    return "NumSubtract";
                case NumAdd:         return "NumAdd";
                case NumEnter:       return "NumEnter";
                case NumEqual:       return "NumEqual";
                
                case LeftShift:      return "LeftShift";
                case LeftCtrl:       return "LeftCtrl";
                case LeftAlt:        return "LeftAlt";
                case LeftSuper:      return "LeftSuper";
                case RightShift:     return "RightShift";
                case RightCtrl:      return "RightCtrl";
                case RightAlt:       return "RightAlt";
                case RightSuper:     return "RightSuper";
                case Menu:           return "Menu";
            }
            return "Unknown";
        }
        
        inline std::ostream& operator<<(std::ostream& os, const Key& k)
        {
            return os << KeyString(k);
        }
    }
}
