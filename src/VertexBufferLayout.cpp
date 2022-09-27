/**
 * Grafik
 * VertexBufferLayout
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "VertexBufferLayout.h"

#include <stdexcept>


template<typename T>
void VertexBufferLayout::Push(int count)
{
    throw std::runtime_error("Call with undeclared type");
}

template<>
void VertexBufferLayout::Push<unsigned>(int count)
{
    _elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLuint));
}

template<>
void VertexBufferLayout::Push<unsigned char>(int count)
{
    _elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLubyte));
}

template<>
void VertexBufferLayout::Push<float>(int count)
{
    _elements.push_back({ GL_FLOAT, count, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLfloat));
}
