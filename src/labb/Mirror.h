/**
 * Grafik
 * Lab: Mirror
 * Copyright 2012-2022 Martin Furuberg
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "renderer/Shader.h"
#include "Texture.h"


namespace labb
{
    class LMirror : public LLab
    {
        float       _speed          { 0.2f };
        float       _colorAlpha     { 0.55f };
        float       _reflectDarken  { 0.85f };
        double      _cycle          { 0 };
        glm::vec3   _cameraPosition { 2.0f, 1.50f, 1.20f };
        bool        _bSpin          { true };
    
    public:
        LMirror();

        void OnTick(TickEvent& e) override;
        void OnRender(RenderEvent& e) override;
        void OnUI(UIEvent& e) override;

    private:
        std::optional<VertexArray> _vao;
        std::shared_ptr<Shader> _shader { nullptr };
        std::optional<Texture> _texture1;
        std::optional<Texture> _texture2;

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
