/**
 * Grafik
 * Event: Keyboard
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "core/Input.h"
#include "events/Event.h"


class KeyEvent : public Event
{
    Input::Keyboard::Key    _key      { 0 };
    Input::InputState       _state    { Input::InputState::Release };
    
public:
    KeyEvent(Input::Keyboard::Key key, Input::InputState state, void* instigator = nullptr)
        : _key { key }, _state { state }
    {
        _instigator = instigator;
    }
    
    GK_EVENT_CLASS_TYPE(Key)
    GK_EVENT_CLASS_CATEGORY(Input | Keyboard)

    [[nodiscard]] Input::Keyboard::Key GetKey() const { return _key; }
    [[nodiscard]] bool IsPressed() const { return _state == Input::InputState::Press; }
    [[nodiscard]] bool IsRepeated() const { return _state == Input::InputState::Repeat; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName();
        stream << " (" << _key << ", ";
        switch (_state)
        {
            case Input::InputState::Press:
                stream << "Press"; break;
            case Input::InputState::Release:
                stream << "Release"; break;
            case Input::InputState::Repeat:
                stream << "Repeat"; break;
        }
        stream << ")";
        return stream.str();
    }
};

//TODO: Handle unicode
class KeyCharEvent : public Event
{
    int  _codePoint { 0 };
    char _char      { 0 };
    
public:
    KeyCharEvent(const int codePoint, void* instigator = nullptr)
        : _codePoint { codePoint }
        , _char { static_cast<char>(codePoint) }
    {
        _instigator = instigator;
    }
    
    GK_EVENT_CLASS_TYPE(KeyChar)
    GK_EVENT_CLASS_CATEGORY(Input | Keyboard)

    [[nodiscard]] char GetChar() const { return _char; }
    [[nodiscard]] int GetCode() const { return _codePoint; }

    [[nodiscard]] std::string ToString() const override
    {
        std::stringstream stream;
        stream << GetName() << " (" << _codePoint << ": " << _char << ")";
        return stream.str();
    }
};
