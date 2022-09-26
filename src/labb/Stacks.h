/**
 * Grafik
 * Lab: Stacks
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <optional>


namespace labb
{
    class LStacks : public LLab
    {
        int         _count        { 25 };
        float       _radius       { 0.5f };
        float       _speed        { 0.7f };
        double      _cycle        { 0 };
        glm::vec4   _color        { 1.0f };
        bool        _bDoCycle     { true };
        bool        _bCycleColor  { true };
    
    public:
        LStacks(Renderer& rr);
        ~LStacks() override;

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    private:
        std::optional<VertexArray> _vao {};
        std::optional<Shader> _shader {};
        std::optional<Texture> _texture {};

        // Matrices
        glm::mat4 _projection {};
        glm::mat4 _view {};
        glm::mat4 _model {};
        glm::mat4 _mvp {};
    };
}
