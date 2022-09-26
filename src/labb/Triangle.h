/**
 * Grafik
 * Lab: Triangle
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "Shader.h"

#include <optional>


namespace labb
{
    class LTriangle : public LLab
    {
        glm::vec3 _rotation { 0.0f, 0.0f, 0.0f };
        
    public:
        LTriangle(Renderer& rr);
        ~LTriangle() override;

        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;
        void BeginUpdate(double DeltaTime) override;

        std::optional<VertexArray> _vao {};
        std::optional<Shader> _triangleShader {};

        // Matrices
        glm::mat4 projection { 1.0f };
        glm::mat4 view { 1.0f };
        glm::mat4 model { 1.0f };
        glm::mat4 mvp { 1.0f };
    };
}
