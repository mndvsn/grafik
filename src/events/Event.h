/**
 * Grafik
 * Event
 * Copyright 2023 Martin Furuberg 
 */
#pragma once


#define GK_EVENT_CLASS_TYPE(type) virtual const char* GetName() const override { return #type; }

class Event
{
public:
    enum class Type : int
    {
        None = 0,
        WindowSize, WindowMinimize, WindowClose,
        Tick, Render, UI,
        Key, KeyChar,
        MouseButton, MouseMove, MouseScroll
    };

    enum Category : int
    {
        None = 0,
        Application,
        Input,
        Keyboard,
        Mouse,
        MouseButton
    };

    virtual ~Event() = default;

    [[nodiscard]] virtual const char* GetName() const = 0;
    [[nodiscard]] virtual std::string ToString() const { return GetName(); }

protected:
    bool _bHandled { false };
};

class EventDispatcher
{
    EventDispatcher() = default;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    os << e.ToString();
    return os;
}
