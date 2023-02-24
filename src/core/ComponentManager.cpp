/**
 * Grafik
 * LayerManager
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "ComponentManager.h"

#include "events/EventManager.h"


void ComponentManager::Attach(const std::shared_ptr<Component> comp)
{
    _comps.emplace_back(comp);

    const auto manager = EventManager::Get();
    int& categoryMask = manager->addListener(comp.get(), GK_BIND_EVENT_HANDLER_EXTERN(comp, OnEvent));
    comp->events = manager;

    comp->OnAttach(categoryMask);
}

void ComponentManager::Detach(const std::shared_ptr<Component> comp)
{
    const auto result = std::ranges::find(_comps, comp);
    if (result != _comps.end())
    {
        const auto manager = EventManager::Get();
        manager->removeListener(comp.get());
        _comps.erase(result);
        comp->OnDetach();
    }
}

bool ComponentManager::Clean()
{
    bool cleaned { false };
    for (const std::shared_ptr<Component>& component : _comps)
    {
        if (component && !component->IsAlive())
        {
            Detach(component);
            cleaned = true;
        }
    }
    return cleaned;
}

ComponentManager::~ComponentManager()
{
    for (const std::shared_ptr<Component>& component : _comps)
    {
        component->OnDetach();
    }
}
