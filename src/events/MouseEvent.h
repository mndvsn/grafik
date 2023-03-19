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
