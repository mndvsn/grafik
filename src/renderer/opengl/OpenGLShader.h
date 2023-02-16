/**
 * Grafik
 * OpenGL Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include "renderer/Shader.h"

#include <glm/fwd.hpp>

#include <string>
#include <vector>
#include <unordered_map>


class OpenGLShader : public Shader
{
    unsigned _id { 0 };
    bool _compiled { false };
    std::string _shaderName { };
    std::string _vertexFilePath { };
    std::string _fragmentFilePath { };
    mutable std::unordered_map<std::string, int> _uniformLocations;
    
public:
    OpenGLShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);
    ~OpenGLShader() override;

    unsigned GetId() const override { return _id; }
    bool IsCompiled() const override { return _compiled; }

    bool Bind() const override;
    static void Unbind();

    // Uniforms
    void SetUniform1i(const std::string& name, int value) const override;
    void SetUniform1iv(const std::string& name, const std::vector<int>& values) const override;
    void SetUniform1f(const std::string& name, float value) const override;
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) const override;
    void SetUniformVec3f(const std::string& name, const glm::vec3& value) const override;
    void SetUniformVec4f(const std::string& name, const glm::vec4& value) const override;
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const override;

    int GetUniformLocation(const std::string& name) const;

private:
    unsigned CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned CompileShaderSource(unsigned type, const std::string& source) const;
};
