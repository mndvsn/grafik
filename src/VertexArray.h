/**
 * Grafik
 * VertexArray
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once

class ElementBuffer;
class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
    unsigned _id { 0 };
    int _elementCount { 0 };

public:
    VertexArray();
    ~VertexArray();
    
    unsigned GetId() const { return _id; }
    
    void Bind() const;
    static void Unbind();
    
    void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddElementBuffer(const ElementBuffer& ebo);
    
    int GetElementCount() const { return _elementCount; }
};
