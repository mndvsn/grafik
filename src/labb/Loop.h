/**
 * Grafik
 * Lab: Loop
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "Lab.h"

#include "DataTexture.h"
#include "renderer/Shader.h"
#include "VertexArray.h"

#include <array>


namespace labb
{
    constexpr unsigned loopSegments { 32 };
    constexpr size_t loopVerticesCount { 4 * loopSegments};
    constexpr size_t loopIndicesCount { 6 * loopSegments};

    class LLoop : public LLab
    {
        float       _speed          { -0.1f };
        double      _cycle          { 0 };
        glm::vec3   _modelRotation  { 335.0f, 0.0f, 350.0f };
        glm::vec3   _cameraPosition { 0.0f, 0.0f, -6.9f };
        bool        _bSpin          { true };
        glm::vec4   _fgColor        { 0.322, 0.247, 0.855, 1.0f };
        glm::vec3   _bgColor        { 0.1f };
        int         _texId          { 0 };

    public:
        LLoop();

        void BeginUpdate(double DeltaTime) override;
        void BeginRender() override;
        void BeginGUI(bool* bKeep) override;

    protected:
        static std::array<Vertex, loopVerticesCount> MakeCylinder(glm::vec2 origin = { 0.0f, 0.0f }, float radius = 1.0f, float length = 1.0f, int segments = 10);

    private:
        VertexArray _vao {};
        std::shared_ptr<Shader> _shader { Shader::Create( "data/shaders/loop.vert", "data/shaders/loop.frag" ) };
        Texture _texture0 { "data/textures/loop_alpha_inv.png" };
        Texture _texture1 { "data/textures/loop_alpha.png" };
        Texture _texture2 { "data/textures/loop.png" };

        // Matrices
        glm::mat4 _projection { 1.0f };
        glm::mat4 _view { 1.0f };
        glm::mat4 _model { 1.0f };
        glm::mat4 _mvp { 1.0f };
    };
}
