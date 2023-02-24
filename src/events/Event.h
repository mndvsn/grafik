/**
 * Grafik
 * Event
 * Copyright 2023 Martin Furuberg
 */
#pragma once


#define GK_BIND_EVENT_HANDLER_EXTERN(obj,h) [obj](auto&& ... args) -> decltype(auto) { (obj)->h(std::forward<decltype(args)>(args)...); }
#define GK_BIND_EVENT_HANDLER(h) GK_BIND_EVENT_HANDLER_EXTERN(this,h)

#define GK_EVENT_CLASS_TYPE(type) \
    [[nodiscard]] static Type GetStaticType() { return Event::Type::type; } \
    [[nodiscard]] virtual Type GetEventType() const override { return GetStaticType(); } \
    [[nodiscard]] virtual const char* GetName() const override { return #type; }

#define GK_EVENT_CLASS_CATEGORY(category) \
    [[nodiscard]] virtual int GetCategories() const override { return category; } 

class Event
{
public:
    enum class Type : int
    {
        None = 0,
        InitLab,
        WindowSize, WindowMinimize, WindowClose,
        Tick, Render, UI,
        Key, KeyChar,
        MouseButton, MouseMove, MouseScroll
    };

    enum Category : int
    {
        None = 0,
        Application         = BIT(0),
        Input               = BIT(1),
        Keyboard            = BIT(2),
        Mouse               = BIT(3),
        MouseButton         = BIT(4)
    };

    virtual ~Event() = default;

    [[nodiscard]] virtual const char* GetName() const = 0;
    [[nodiscard]] virtual Type GetEventType() const = 0;
    [[nodiscard]] virtual int GetCategories() const = 0;
    [[nodiscard]] virtual std::string ToString() const { return GetName(); }

    [[nodiscard]] bool IsCategory(Category category) const { return GetCategories() & category; }
    
    virtual void Handled() { _bHandled = true; }
    [[nodiscard]] bool IsHandled() const { return _bHandled; }

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
