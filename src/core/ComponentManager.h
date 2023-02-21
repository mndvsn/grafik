/**
 * Grafik
 * ComponentManager
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "components/Component.h"


class ComponentManager
{
    std::vector<std::shared_ptr<Component>> _comps { };

public:
    ComponentManager() = default;
    ~ComponentManager();

    void Attach(std::shared_ptr<Component> comp);
    void Detach(std::shared_ptr<Component> comp);

    void Clean();

    [[nodiscard]] std::vector<std::shared_ptr<Component>>::iterator begin() { return _comps.begin(); }
    [[nodiscard]] std::vector<std::shared_ptr<Component>>::iterator end() { return _comps.end(); }
    [[nodiscard]] std::vector<std::shared_ptr<Component>>::const_iterator begin() const { return _comps.begin(); }
    [[nodiscard]] std::vector<std::shared_ptr<Component>>::const_iterator end() const { return _comps.end(); }
};
