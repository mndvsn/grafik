/**
* Grafik
 * IndexBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "IndexBuffer.h"

#include <glad/glad.h>

IndexBuffer::IndexBuffer(const unsigned* data, unsigned count)
    : _count(count)
{
    glGenBuffers(1, &_id);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * static_cast<signed long long>(sizeof(unsigned)), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
