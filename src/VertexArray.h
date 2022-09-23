/**
 * Grafik
 * VertexArray
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
private:
    unsigned _id;

public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    static void Unbind();
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};
