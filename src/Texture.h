/**
 * Grafik
 * Texture
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <string>

class Texture
{
    unsigned _id { 0 };
    std::string _filePath {};
    unsigned char* _localBuffer { nullptr };
    bool _loaded { false };
    int _width { 0 };
    int _height { 0 };
    int _bpp { 0 };

public:
    Texture(const std::string& filePath);
    ~Texture();

    bool Bind(unsigned unit = 0) const;
    void Unbind() const;

    unsigned GetId() const { return _id; }
    bool IsOK() const { return _loaded; }

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    std::string GetPath() const { return _filePath; }
};
