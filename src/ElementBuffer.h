/**
 * Grafik
 * ElementBuffer
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once


class ElementBuffer
{
private:
    unsigned _id { 0 };
    unsigned _count { 0 };
    
public:
    ElementBuffer(const unsigned* data, unsigned count);
    ~ElementBuffer();

    unsigned GetId() const { return _id; }

    void Bind() const;
    static void Unbind();

    unsigned GetCount() const { return _count; }
};
