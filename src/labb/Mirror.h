/**
 * Grafik
 * Lab: Mirror
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
    class LMirror : public LLab
    {
        float       _speed          { 0.2f };
        float       _colorAlpha     { 0.4f };
        float       _reflectDarken  { 0.85f };
        double      _cycle          { 0 };
        glm::vec3   _cameraPosition { 2.0f, 1.50f, 1.20f };
        bool        _bSpin          { true };
    
    public:
        LMirror(Renderer& rr);

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    private:
        std::optional<VertexArray> _vao;
        std::optional<Shader> _shader;
        std::optional<Texture> _texture;

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
