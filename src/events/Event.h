/**
 * Grafik
 * Event
 * Copyright 2023 Martin Furuberg
 */
#pragma once


#define GK_BIND_EVENT_HANDLER(h) [this](auto&& ... args) -> decltype(auto) { this->h(std::forward<decltype(args)>(args)...); }

#define GK_EVENT_CLASS_TYPE(type) \
    static Type GetStaticType() { return Event::Type::type; } \
    virtual Type GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; } \

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
    [[nodiscard]] virtual Type GetEventType() const = 0;
    [[nodiscard]] virtual std::string ToString() const { return GetName(); }

    virtual void Handled() { _bHandled = true; }

protected:
    bool _bHandled { false };
    friend class EventDispatcher;
};

class EventDispatcher
{
    Event& _event;

public:
    EventDispatcher(Event& event)
        : _event { event } { }

    template <typename T, typename H>
    void Dispatch(const H& handler)
    {
        if (!_event._bHandled && _event.GetEventType() == T::GetStaticType())
        {
            handler(static_cast<T&>(_event));
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    os << e.ToString();
    return os;
}
