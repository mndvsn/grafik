﻿/**
 * Grafik
 * VertexBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once


class VertexBuffer
{
private:
    unsigned _id { 0 };
    
public:
    VertexBuffer(const void* data, unsigned size);
    ~VertexBuffer();

    void Bind() const;
    static void Unbind();
};
