/**
 * Grafik
 * VertexArray
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &_id);
    Bind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &_id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(_id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // Bind this VAO
    Bind();

    // Bind VertexBuffer
    vb.Bind();
    
    // Define each attribute (element) in layout
    for (auto& element : layout.GetElements())
    {
        glVertexAttribPointer(element.location, element.count, element.type, element.normalized, layout.GetStride(),
            reinterpret_cast<const void*>(static_cast<intptr_t>(element.offset)));  // NOLINT(performance-no-int-to-ptr)
        glEnableVertexAttribArray(element.location);
    }
}
