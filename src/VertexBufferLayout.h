/**
 * Grafik
 * VertexBufferLayout
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdexcept>


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

template<typename T>
inline void VertexBufferLayout::Push(int count)
{
    throw std::runtime_error("Call with undeclared type");
}

template<>
inline void VertexBufferLayout::Push<unsigned>(int count)
{
    _elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLuint));
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(int count)
{
    _elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLubyte));
}

template<>
inline void VertexBufferLayout::Push<float>(int count)
{
    _elements.push_back({ GL_FLOAT, count, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * static_cast<int>(sizeof(GLfloat));
}

template<>
inline void VertexBufferLayout::Push<glm::vec2>(int count)
{
    _elements.push_back({ GL_FLOAT, count * 2, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * 2 * static_cast<int>(sizeof(GLfloat));
}

template<>
inline void VertexBufferLayout::Push<glm::vec3>(int count)
{
    _elements.push_back({ GL_FLOAT, count * 3, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * 3 * static_cast<int>(sizeof(GLfloat));
}

template<>
inline void VertexBufferLayout::Push<glm::vec4>(int count)
{
    _elements.push_back({ GL_FLOAT, count * 4, GL_FALSE, _stride, static_cast<int>(_elements.size()) });
    _stride += count * 4 * static_cast<int>(sizeof(GLfloat));
}
