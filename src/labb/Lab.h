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
#include <optional>
#include <string>
#include <vector>


namespace labb
{
    struct Vertex
    {
        glm::vec3   Position    { 0.0f, 0.0f, 0.0f };
        glm::vec4   Color       { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec2   TexCoords   { 0.0f, 0.0f };
        float       TexId       { 0.0f };
    };

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
        std::vector<std::pair<std::string, LLabMenuItem>> _labs;
    
    public:
        LLabMenu(Renderer& renderer, std::unique_ptr<LLab>& activeLabPtr);

        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;
        void BeginLabMenu();
        void BeginBigMenu();

        template<typename T>
        void RegisterLab(const std::string& name, const std::string& shortName)
        {
            std::cout << "Registering lab: " << name << std::endl;
            _labs.push_back({ shortName,
                {
                    name, [this] { return new T(_renderer); }
                }
            });
        }

        std::optional<LLab*> CreateLabIfExists(const std::string& labShortName);
    };
}

