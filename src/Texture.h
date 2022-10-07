/**
 * Grafik
 * Texture
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <string>

class Texture
{
    std::string _filePath {};
    unsigned char* _localBuffer { nullptr };
    int _bpp { 0 };

protected:
    unsigned _id { 0 };
    bool _loaded { false };
    int _width { 0 };
    int _height { 0 };

public:
    Texture() = default;
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
