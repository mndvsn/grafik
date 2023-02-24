/**
 * Grafik
 * EventManager
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "EventManager.h"


EventManager* EventManager::Get()
{
    std::lock_guard lock(_mutex);
    if (!_manager)
    {
        _manager = new EventManager();
    }
    return _manager;
}

int& EventManager::addListener(const void* object, const EventCallbackFunc& func, int categoryMask)
{
    const auto listener = _listeners.emplace_back(std::make_shared<EventHandle>(object, func, categoryMask));
    return listener->categoryMask;
}

bool EventManager::removeListener(const void* object)
{
    auto listenerObject = [&object](const std::shared_ptr<EventHandle>& cl) { return cl->object == object; };
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
    for (size_t i = 0; i < _listeners.size(); i++)
    {
        if (event.IsHandled()) break;

        const auto& listener = _listeners[i];
        if (listener && listener->categoryMask & event.GetCategories())
        {
            listener->func(event);
        }
    }
}

EventManager::~EventManager()
{
    delete _manager;
}
