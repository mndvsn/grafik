/**
 * Grafik
 * VertexBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "VertexBuffer.h"

#include <glad/glad.h>
VertexBuffer::VertexBuffer(const void* data, unsigned size)

{
    glGenBuffers(1, &_id);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
