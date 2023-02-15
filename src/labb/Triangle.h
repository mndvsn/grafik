/**
 * Grafik
 * Lab: Triangle
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "renderer/Shader.h"

#include <optional>


namespace labb
{
    class LTriangle : public LLab
    {
        glm::vec3 _rotation { 0.0f, 0.0f, 0.0f };
        
    public:
        LTriangle();

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    private:
        std::optional<VertexArray> _vao;
        std::shared_ptr<Shader> _triangleShader { nullptr };

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
