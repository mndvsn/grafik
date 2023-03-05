/**
 * Grafik
 * Event: Application
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "events/Event.h"


namespace labb { class LLab; }

class WindowSizeEvent : public Event
{
public:
    WindowSizeEvent(unsigned width, unsigned height)
        : _width { width }, _height { height } { }
    
    GK_EVENT_CLASS_TYPE(WindowSize)
    GK_EVENT_CLASS_CATEGORY(Application)

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
    GK_EVENT_CLASS_CATEGORY(Application)
};

using LabFactoryFunc = std::function<labb::LLab*()>;

class InitLabEvent : public Event
{
public:
    InitLabEvent(LabFactoryFunc fn)
        : createLab { std::move(fn) } { }
    
    GK_EVENT_CLASS_TYPE(InitLab)
    GK_EVENT_CLASS_CATEGORY(Application)

    LabFactoryFunc createLab;
};

class TickEvent : public Event
{
public:
    TickEvent(double dt)
        : _deltaTime { dt } { }

    [[nodiscard]] double GetDeltaTime() const { return _deltaTime; }
    
    GK_EVENT_CLASS_TYPE(Tick)
    GK_EVENT_CLASS_CATEGORY(Application)

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName() << " (" << _deltaTime << ")";
        return stream.str();
    }

private:
    double _deltaTime { };
};

class RenderEvent : public Event
{
public:
    RenderEvent() = default;

    GK_EVENT_CLASS_TYPE(Render)
    GK_EVENT_CLASS_CATEGORY(Application)
};

class FramebufferSizeEvent : public Event
{
public:
    FramebufferSizeEvent(unsigned width, unsigned height)
        : _width { width }, _height { height } { }

    GK_EVENT_CLASS_TYPE(FramebufferSize)
    GK_EVENT_CLASS_CATEGORY(Application)

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

class UIEvent : public Event
{
public:
    UIEvent() = default;

    GK_EVENT_CLASS_TYPE(UI)
    GK_EVENT_CLASS_CATEGORY(Application)
};