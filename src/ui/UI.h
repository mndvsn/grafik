/**
 * Grafik
 * UI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once
#include <memory>


struct GLFWwindow;

class UI
{
public:
    virtual ~UI() = default;
    
    virtual void Init(GLFWwindow* window) = 0;

    virtual void Begin() = 0;
    virtual void End() = 0;

    static std::unique_ptr<UI> Create();
};
