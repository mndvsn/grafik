/**
 * Grafik
 * IndexBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once


class IndexBuffer
{
private:
    unsigned _id { 0 };
    unsigned _count { 0 };
    
public:
    IndexBuffer(const unsigned* data, unsigned count);
    ~IndexBuffer();

    void Bind() const;
    static void Unbind();

    unsigned GetCount() const { return _count; }
};
