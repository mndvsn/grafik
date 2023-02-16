/**
 * Grafik
 * Texture
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb/stb_image.h>


Texture::Texture(const std::string& filePath) : _filePath { filePath }
{
    // Load texture from image file
    stbi_set_flip_vertically_on_load(1);
    _localBuffer = stbi_load(filePath.c_str(), &_width, &_height, &_bpp, 4);
    
    if (_localBuffer)
    {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        _loaded = true;
    }
    else
    {
        std::cout << "Error: Failure loading '" << _filePath << "'; " << stbi_failure_reason() << std::endl;
    }
    
    stbi_image_free(_localBuffer);
    Unbind();
}

Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}

bool Texture::Bind(unsigned unit) const
{
    if (IsOK())
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, _id);
        return true;
    }
    return false;
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
