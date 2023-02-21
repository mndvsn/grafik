/**
 * Grafik
 * EventManager
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "events/Event.h"

#include <mutex>


using EventCallbackFunc = std::function<void(Event&)>;

struct EventHandle
{
    const void* object { nullptr };
    const EventCallbackFunc func { };
    int categoryMask { };

    bool operator==(const EventHandle& handle) const { return object == handle.object; }
};

class EventManager
{
protected:
    EventManager() = default;
    ~EventManager();

public:
    EventManager(EventManager&) = delete;
    void operator=(const EventManager&) = delete;

    static EventManager* Get();

    int& addListener(const void* object, const EventCallbackFunc& func, int categoryMask = 0);
    bool removeListener(const void* object);

    void Broadcast(Event& event) const;
    
private:
    std::vector<std::shared_ptr<EventHandle>> _listeners { };
    inline static EventManager* _manager { nullptr };
    inline static std::mutex _mutex { };
};
