/**
 * Grafik
 * Event: Mouse
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "core/Input.h"
#include "events/Event.h"


class MouseButtonEvent : public Event
{
    Input::Mouse::Button    _button     { 0 };
    Input::InputState       _pressed    { Input::InputState::Release };
    
public:
    MouseButtonEvent(int button, bool pressed, void* instigator = nullptr)
        : _button { static_cast<Input::Mouse::Button>(button) }
        , _pressed { pressed ? Input::InputState::Press : Input::InputState::Release }
    {
        _instigator = instigator;
    }
    
    GK_EVENT_CLASS_TYPE(MouseButton)
    GK_EVENT_CLASS_CATEGORY(Input | Mouse | MouseButton)

    [[nodiscard]] Input::Mouse::Button GetButton() const { return _button; }
    [[nodiscard]] bool IsPressed() const { return _pressed == Input::InputState::Press; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName();
        stream << " (" << _button << ", ";
        stream << (_pressed == Input::InputState::Press ? "Pressed" : "Released") << ")";
        return stream.str();
    }
};

class MouseMoveEvent : public Event
{
    uint16_t _mouseX { 0 };
    uint16_t _mouseY { 0 };
    
public:
    MouseMoveEvent(const uint16_t x, const uint16_t y, void* instigator = nullptr)
        : _mouseX { x }, _mouseY { y }
    {
        _instigator = instigator;
    }

    GK_EVENT_CLASS_TYPE(MouseMove)
    GK_EVENT_CLASS_CATEGORY(Input | Mouse)

    [[nodiscard]] uint16_t GetX() const { return _mouseX; }
    [[nodiscard]] uint16_t GetY() const { return _mouseX; }
    [[nodiscard]] std::pair<uint16_t, uint16_t> GetPosition() const { return { _mouseX, _mouseY }; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName();
        stream << " (x: " << _mouseX << ", y: " << _mouseY << ")";
        return stream.str();
    }
};

class MouseScrollEvent : public Event
{
    int16_t _deltaX { 0 };
    int16_t _deltaY { 0 };
    
public:
    MouseScrollEvent(const int16_t dx, const int16_t dy, void* instigator = nullptr)
        : _deltaX { dx }, _deltaY { dy }
    {
        _instigator = instigator;
    }

    GK_EVENT_CLASS_TYPE(MouseScroll)
    GK_EVENT_CLASS_CATEGORY(Input | Mouse)

    [[nodiscard]] uint16_t GetScrollX() const { return _deltaX; }
    [[nodiscard]] uint16_t GetScrollY() const { return _deltaY; }
    [[nodiscard]] std::pair<int16_t, int16_t> GetScroll() const { return { _deltaX, _deltaY }; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName();
        stream << " (x: " << _deltaX << ", y: " << _deltaY << ")";
        return stream.str();
    }
};
