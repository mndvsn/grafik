/**
 * Grafik
 * LayerManager
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "ComponentManager.h"

#include "events/EventManager.h"

#include <ranges>


void ComponentManager::Attach(Component* comp)
{
    _comps.emplace_back(comp);

    const auto manager = EventManager::Get();
    int& categoryMask = manager->AddListener(comp, GK_BIND_EVENT_HANDLER_EXTERN(comp, OnEvent), 0, true);
    comp->events = manager;

    Log::Msg("Component attached");
    comp->OnAttach(categoryMask);
}

void ComponentManager::Detach(Component* comp)
{
    auto compare = [&comp](const std::unique_ptr<Component>& c) { return c.get() == comp; };
    const auto result = std::ranges::find_if(_comps, compare);
    if (result != _comps.end())
    {
        Log::Msg("Component detached");
        comp->OnDetach();
        _comps.erase(result);
    }
}

bool ComponentManager::Clean()
{
    bool cleaned { false };
    for (const auto& component : _comps)
    {
        if (component && !component->IsAlive())
        {
            Detach(component.get());
            cleaned = true;
        }
    }
    return cleaned;
}

ComponentManager::~ComponentManager()
{
    // reverse order
    for (auto& component : std::ranges::reverse_view { _comps })
    {
        component->OnDetach();
        component.reset();
    }
}
