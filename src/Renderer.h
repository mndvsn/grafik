/**
 * Grafik
 * Renderer
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once


class VertexArray;
class Shader;

class Renderer
{
public:
    void Render(const VertexArray& vao, const Shader& shader) const;

    void Clear() const;
    void SetWireframeMode(bool bUseLineDraw);
};
