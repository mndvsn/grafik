/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include "Renderer.h"

#include <functional>
#include <iostream>
#include <string>


namespace labb
{
    class LLab
    {
    public:
        LLab(Renderer& renderer);
        virtual ~LLab();

        virtual void BeginRender();
        virtual void BeginGUI(bool* bKeep);
        virtual void BeginUpdate(double DeltaTime);

        [[nodiscard]] Renderer& GetRenderer() const { return _renderer; }
        
    protected:
        Renderer& _renderer;
        
        void RenderError(const std::string& error);
        
    private:
        bool _bHasError { false };
        std::string _errorString { };
    };

    struct LLabMenuItem
    {
        std::string name;
        std::function<LLab*()> createInstance;
    };

    class LLabMenu : public LLab
    {
        std::unique_ptr<LLab>& _activeLab;
        std::vector<LLabMenuItem> _labs;
    
    public:
        LLabMenu(Renderer& renderer, std::unique_ptr<LLab>& activeLabPtr);

        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;
        void BeginLabMenu();

        template<typename T>
        void RegisterLab(const std::string& name)
        {
            std::cout << "Registering lab: " << name << std::endl;

            _labs.push_back({ name,
                [this]
                {
                    return new T(_renderer);
                }
            });
        }
    };
}

