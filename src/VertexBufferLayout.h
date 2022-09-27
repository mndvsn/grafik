/**
 * Grafik
 * VertexBufferLayout
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>

#include <vector>


struct VertexBufferElement
{
    unsigned type { GL_FLOAT };
    int count { 0 };
    unsigned char normalized { GL_FALSE };
    int offset { 0 };
    int location { 0 };
};

class VertexBufferLayout
{
private:
  std::vector<VertexBufferElement> _elements {};
  int _stride { 0 };

public:
    VertexBufferLayout() = default;
    ~VertexBufferLayout() = default;

    template<typename T>
    void Push(int count);

    const std::vector<VertexBufferElement>& GetElements() const { return _elements; }
    
    int GetStride() const { return _stride; }
};