/**
 * Grafik
 * DataTexture
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "DataTexture.h"

#include <glad/glad.h>


DataTexture::DataTexture(bool isWhite)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    const unsigned color { isWhite ? 0xFFFFFFFF : 0x00000000 };
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    _loaded = true;
    Unbind();
}
