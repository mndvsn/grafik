/**
 * Grafik
 * Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Shader.h"
#include "utils/File.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <filesystem>
#include <iostream>


Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
    : _vertexFilePath { vertexFile }, _fragmentFilePath { fragmentFile }
{
    _shaderName = ExtractName(vertexFile);
    
    // Read vertex shader from file
    File vsFile(vertexFile.c_str());
    const std::string vertexSource = vsFile.Read();
    
    // Read fragment shader from file
    File fsFile(fragmentFile.c_str());
    const std::string fragmentSource = fsFile.Read();

    // Compile into program
    _id = CreateShaderProgram(vertexSource, fragmentSource);
}

unsigned Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    const unsigned program = glCreateProgram();
    const unsigned vs = CompileShaderSource(GL_VERTEX_SHADER, vertexShader);
    const unsigned fs = CompileShaderSource(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned Shader::CompileShaderSource(unsigned type, const std::string& source)
{
    const unsigned id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check for errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length {};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Error: Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader in '" << _shaderName << "' failed:\n";
        std::cout << "\t" << message << "\n";

        glDeleteShader(id);
        return 0;
    }
    return id;
}

std::string Shader::ExtractName(const std::string& filePath) const
{
    const std::filesystem::path path(filePath);
    return path.stem().string();
}

void Shader::Bind() const
{
    if (_id == 0)
    {
        std::cout << "Warning: Could not use shader '" << _shaderName << ". Shader not compiled.\n";
        return;
    }
    
    glUseProgram(_id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& value)
{
    glUniform4fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0].x);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (_uniformLocations.contains(name))
    {
        return _uniformLocations[name];
    }
    
    const int location = glGetUniformLocation(_id, name.c_str());
    if (location < 0)
    {
        std::cout << "Warning: Uniform '" << name << "' not found in shader '" << _shaderName << "'.\n";
    }
    _uniformLocations[name] = location;
    
    return location;
}

Shader::~Shader()
{
    glDeleteProgram(_id);
}
