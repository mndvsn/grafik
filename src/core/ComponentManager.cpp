/**
 * Grafik
 * LayerManager
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "ComponentManager.h"

#include "events/EventManager.h"


void ComponentManager::Attach(std::shared_ptr<Component> comp)
{
    _comps.emplace_back(comp);

    const auto manager = EventManager::Get();
    int& categoryMask = manager->addListener(comp.get(), GK_BIND_COMPONENT_EVENT_HANDLER(comp, OnEvent));
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

void ComponentManager::Clean()
{
    for (const std::shared_ptr<Component>& component : _comps)
    {
        if (!component->IsAlive())
        {
            Detach(component);
        }
    }
}

ComponentManager::~ComponentManager()
{
    for (const std::shared_ptr<Component>& component : _comps)
    {
        component->OnDetach();
    }
}
