/**
 * Grafik
 * Lab: Batch
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "DataTexture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "renderer/Shader.h"
#include "Texture.h"

#include <random>


namespace labb
{
    constexpr size_t batchQuadCapacity { 20000 };
    constexpr size_t batchVerticesCount { batchQuadCapacity * 4 };
    constexpr size_t batchIndicesCount { batchQuadCapacity * 6 };

    class LBatch : public LLab
    {
        float       _speed          { 0.2f };
        float       _breakAmount    { 0.0f };
        double      _cycle          { 0 };
        glm::vec3   _cameraPosition { 0.0f, 0.0f, -7.5f };
        bool        _bSpin          { false };
        int         _quads          { 5928 };
        int         _draws          { 0 };
    
    public:
        LBatch();
        ~LBatch() override;

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    protected:
        static Vertex* MakeQuad(Vertex* vertexPtr, float x, float y, float z, float width = 1.0f, float height = 1.0f,
            float texId = 0.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

    private:
        VertexArray _vao {};
        VertexBuffer _vbo { nullptr, sizeof(Vertex) * batchVerticesCount, true };
        std::shared_ptr<Shader> _shader { Shader::Create( "data/shaders/batch.vert", "data/shaders/batch.frag" ) };
        std::optional<DataTexture> _texture0;
        std::optional<Texture> _texture1;
        std::optional<Texture> _texture2;

        Vertex* _vertices { nullptr };

        unsigned _seed {};
        std::default_random_engine randomEngine {};
        std::uniform_int_distribution<int> randomizer { 0, 2 };

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };

        void RandomizeSeed();
    };
}
