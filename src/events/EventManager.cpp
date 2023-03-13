﻿/**
 * Grafik
 * EventManager
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "EventManager.h"

#include "components/Component.h"


EventManager* EventManager::Get()
{
    std::lock_guard lock(_mutex);
    if (!_manager)
    {
        _manager = new EventManager();
    }
    return _manager;
}

int& EventManager::AddListener(const void* object, const EventCallbackFunc& func, int categoryMask, bool isComponent)
{
    const auto& listener = _listeners.emplace_back(std::make_unique<EventHandle>(object, func, categoryMask, isComponent));
    return listener->categoryMask;
}

bool EventManager::RemoveListener(const void* object)
{
    auto listenerObject = [&object](const std::unique_ptr<EventHandle>& cl) { return cl->object == object; };
    const auto match = std::ranges::find_if(_listeners, listenerObject);
    if (match != _listeners.end())
    {
        _listeners.erase(match);
        return true;
    }
    return false;
}

void EventManager::Broadcast(Event& event) const
{
    for (int i = static_cast<int>(_listeners.size()); i --> 0;)
    {
        if (event.IsHandled()) break;

        const auto& listener = _listeners[i];
        if (listener && listener->categoryMask & event.GetCategories())
        {
            // Skip if Listener object is component and disabled
            if (listener->bComponent && !static_cast<const Component*>(listener->object)->IsEnabled())
            {
                continue;
            }
            listener->func(event);
        }
    }
}

void EventManager::Reset()
{
    _listeners.clear();
}
