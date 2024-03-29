﻿/**
 * Grafik
 * Lab: Stacks
 * Copyright 2012-2022 Martin Furuberg
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "renderer/Shader.h"
#include "Texture.h"


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
        LStacks();

        void OnTick(TickEvent& e) override;
        void OnRender(RenderEvent& e) override;
        void OnUI(UIEvent& e) override;

    private:
        std::optional<VertexArray> _vao;
        std::shared_ptr<Shader> _shader { nullptr };
        std::optional<Texture> _texture;

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
