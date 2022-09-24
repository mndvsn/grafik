/**
 * Grafik
 * VertexArray
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "VertexArray.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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

void VertexArray::AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // use one binding point for now
    constexpr unsigned bufferBindingPoint { 0 };
    // get data from start of buffer
    constexpr unsigned offset { 0 };
    
    // Bind this VAO
    Bind();
    
    // Define each attribute (element) in layout
    for (auto& element : layout.GetElements())
    {
        glVertexAttribFormat(element.location, element.count, element.type, element.normalized, element.offset);
        glVertexAttribBinding(element.location, bufferBindingPoint);
        glEnableVertexAttribArray(element.location);
    }
    
    // Bind VertexBuffer to binding point
    glBindVertexBuffer(bufferBindingPoint, vb.GetId(), offset, layout.GetStride());
}

void VertexArray::AddElementBuffer(const ElementBuffer& ebo)
{
    _elementCount = ebo.GetCount();
}
