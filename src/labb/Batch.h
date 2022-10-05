/**
 * Grafik
 * Lab: Batch
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <array>
#include <optional>


namespace labb
{
    constexpr size_t quadCount { 4 };
    constexpr size_t verticesCount { quadCount * 4 };
    constexpr size_t indicesCount { quadCount * 6 };
    
    struct Vertex
    {
        glm::vec3   Position    { 0.0f, 0.0f, 0.0f };
        glm::vec4   Color       { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec2   TexCoords   { 0.0f, 0.0f };
        float       TexId       { 0.0f };
    };
    
    class LBatch : public LLab
    {
        float       _speed          { 0.2f };
        double      _cycle          { 0 };
        glm::vec3   _cameraPosition { 0.0f, 0.0f, -3.0f };
        bool        _bSpin          { true };
        int         _draws          { 0 };
    
    public:
        LBatch(Renderer& rr);

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    protected:
        static std::array<Vertex, 4> MakeQuad(float x, float y, float width = 1.0f, float height = 1.0f, float texId = 0.0f);

    private:
        VertexArray _vao {};
        VertexBuffer _vbo { nullptr, sizeof(Vertex) * verticesCount, true };
        Shader _shader { "data/shaders/batch.vert", "data/shaders/batch.frag" };
        std::optional<Texture> _texture1;
        std::optional<Texture> _texture2;

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
