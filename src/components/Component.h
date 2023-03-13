/**
 * Grafik
 * Component
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "events/EventManager.h"


class Component
{
protected:
    bool _keepAlive { true };
    bool _enabled { true };
    EventManager* events { nullptr };
    
public:
    Component() = default;
    virtual ~Component() { EventManager::Get()->RemoveListener(this); }

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    virtual void OnAttach(int& /*eventMask*/) { }
    virtual void OnDetach() { }
    virtual void OnEvent(Event& /*event*/) { }

    [[nodiscard]] bool IsAlive() const { return _keepAlive; }
    [[nodiscard]] bool IsEnabled() const { return _enabled; }
    
private:
    friend class ComponentManager;
};
