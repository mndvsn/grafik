/**
 * Grafik
 * Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once
#include <glm/fwd.hpp>

#include <memory>
#include <string>
#include <vector>


class Shader
{
public:
    static std::shared_ptr<Shader> Create(const std::string& vertexFile, const std::string& fragmentFile);
    static std::string ExtractName(const std::string& filePath);
    virtual ~Shader() = default;

    [[nodiscard]] virtual unsigned GetId() const = 0;
    [[nodiscard]] virtual bool IsCompiled() const = 0;
    [[nodiscard]] bool IsOK() const { return IsCompiled() && GetId(); }

    virtual bool Bind() const = 0;
    static void Unbind();

    // Uniforms
    virtual void SetUniform1i(const std::string& name, int value) const = 0;
    virtual void SetUniform1iv(const std::string& name, const std::vector<int>& values) const = 0;
    virtual void SetUniform1f(const std::string& name, float value) const = 0;
    virtual void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) const = 0;
    virtual void SetUniformVec3f(const std::string& name, const glm::vec3& value) const = 0;
    virtual void SetUniformVec4f(const std::string& name, const glm::vec4& value) const = 0;
    virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const = 0;
};
