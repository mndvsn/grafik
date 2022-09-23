/**
 * Grafik
 * ElementBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "ElementBuffer.h"

#include <glad/glad.h>

ElementBuffer::ElementBuffer(const unsigned* data, unsigned count)
    : _count(count)
{
    glGenBuffers(1, &_id);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * static_cast<signed long long>(sizeof(unsigned)), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &_id);
}

void ElementBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void ElementBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
