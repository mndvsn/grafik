/**
 * Grafik
 * Event: Application
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "events/Event.h"


class WindowSizeEvent : public Event
{
public:
    WindowSizeEvent(unsigned width, unsigned height)
        : _width { width }, _height { height } { }
    
    GK_EVENT_CLASS_TYPE(WindowSize)

    [[nodiscard]] unsigned GetWidth() const { return _width; }
    [[nodiscard]] unsigned GetHeight() const { return _height; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName() << " (" << _width << ", " << _height << ")";
        return stream.str();
    }

private:
    unsigned _width { 0 }, _height { 0 };
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    GK_EVENT_CLASS_TYPE(WindowClose)
};
