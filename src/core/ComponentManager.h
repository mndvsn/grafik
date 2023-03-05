/**
 * Grafik
 * ComponentManager
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "components/Component.h"


class ComponentManager
{
public:
    ComponentManager() = default;
    ~ComponentManager();

    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    template <class T>
    T* Create(auto&& ... args);
    void Attach(Component* comp);
    void Detach(Component* comp);

    bool Clean();

    [[nodiscard]] size_t GetCount() const { return _comps.size(); } 

    [[nodiscard]] std::vector<std::unique_ptr<Component>>::iterator begin() { return _comps.begin(); }
    [[nodiscard]] std::vector<std::unique_ptr<Component>>::iterator end() { return _comps.end(); }
    [[nodiscard]] std::vector<std::unique_ptr<Component>>::const_iterator begin() const { return _comps.begin(); }
    [[nodiscard]] std::vector<std::unique_ptr<Component>>::const_iterator end() const { return _comps.end(); }

private:
    std::vector<std::unique_ptr<Component>> _comps { };
};

template <class T>
T* ComponentManager::Create(auto&& ... args)
{
    static_assert(std::is_base_of_v<Component, T>); // make sure it's a component
    
    _comps.emplace_back(std::make_unique<T>(std::forward<decltype(args)>(args)...));
    auto comp = static_cast<T*>(_comps.back().get());

    const auto manager = EventManager::Get();
    int& categoryMask = manager->addListener(comp, GK_BIND_EVENT_HANDLER_EXTERN(comp, OnEvent));
    comp->events = manager;

    comp->OnAttach(categoryMask);
    return comp;
}