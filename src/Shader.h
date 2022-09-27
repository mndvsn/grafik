/**
 * Grafik
 * Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glm/fwd.hpp>

#include <string>
#include <unordered_map>


class Shader
{
    unsigned _id { 0 };
    bool _compiled { false };
    std::string _shaderName { };
    std::string _vertexFilePath { };
    std::string _fragmentFilePath { };
    mutable std::unordered_map<std::string, int> _uniformLocations;

public:
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    ~Shader();

    unsigned GetId() const { return _id; }
    bool IsOK() const { return _compiled && _id; }

    bool Bind() const;
    static void Unbind();

    // Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUniformVec3f(const std::string& name, const glm::vec3& value);
    void SetUniformVec4f(const std::string& name, const glm::vec4& value);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    int GetUniformLocation(const std::string& name) const;

private:
    unsigned CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned CompileShaderSource(unsigned type, const std::string& source);
    
    std::string ExtractName(const std::string& filePath) const;
};
