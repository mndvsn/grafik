/**
 * Grafik
 * Pipeline
 * Copyright 2023 Martin Furuberg 
 */
#include "Pipeline.h"
#include "utils/File.h"

#include <filesystem>
#include <iostream>


Pipeline::Pipeline(const std::string& vertexFile, const std::string& fragmentFile)
    : _vertexFilePath { vertexFile }, _fragmentFilePath { fragmentFile }
{
    _shaderName = ExtractName(vertexFile);
    
    // Read vertex shader from file as binary
    File vsFile(vertexFile.c_str());
    const auto vertexSource = vsFile.ReadBytes();

    // Read fragment shader from file as binary
    File fsFile(fragmentFile.c_str());
    const auto fragmentSource = fsFile.ReadBytes();

    if (!vertexSource || !fragmentSource)
    {
        std::cout << "Error: Failed loading source for shader '" << _shaderName << "'." << std::endl;
        return;
    }

    std::cout << "Size vertex: " << vertexSource->size() << std::endl;
    std::cout << "Size fragment: " << fragmentSource->size() << std::endl;

    // *vertexSource, *fragmentSource
}

std::string Pipeline::ExtractName(const std::string& filePath) const
{
    const std::filesystem::path path(filePath);
    return path.stem().string();
}

Pipeline::~Pipeline()
{
    
}
