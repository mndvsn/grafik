﻿/**
 * Grafik
 * Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <string>
#include <unordered_map>


class Shader
{
private:
    unsigned _id { 0 };
    std::string _shaderName { };
    std::string _vertexFilePath { };
    std::string _fragmentFilePath { };
    std::unordered_map<std::string, int> _uniformLocations;

public:
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    ~Shader();

    unsigned GetId() const { return _id; }

    void Bind() const;
    static void Unbind();

    // Uniforms
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    int GetUniformLocation(const std::string& name);
    
private:
    unsigned CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned CompileShaderSource(unsigned type, const std::string& source);
    
    std::string ExtractName(const std::string& filePath) const;
};
