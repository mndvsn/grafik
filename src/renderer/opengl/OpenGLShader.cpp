/**
 * Grafik
 * OpenGL Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "OpenGLShader.h"
#include "utils/File.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <filesystem>


OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
    : _shaderName { name }
    , _vertexFilePath { vertexFile }
    , _fragmentFilePath { fragmentFile }
{
    // Read vertex shader from file
    File vsFile(vertexFile.c_str());
    const auto vertexSource = vsFile.Read();

    // Read fragment shader from file
    File fsFile(fragmentFile.c_str());
    const auto fragmentSource = fsFile.Read();

    if (!vertexSource || !fragmentSource)
    {
        std::cout << "Error: Failed loading source for shader '" << _shaderName << "'." << std::endl;
        return;
    }

    // Compile into program
    _id = CreateShaderProgram(*vertexSource, *fragmentSource);
}

unsigned OpenGLShader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    const unsigned program = glCreateProgram();
    const unsigned vs = CompileShaderSource(GL_VERTEX_SHADER, vertexShader);
    const unsigned fs = CompileShaderSource(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    int valid {};
    glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
    if (valid)
    {
        _compiled = true;
    }
    else
    {
        std::cout << "Warning: Failed to compile shader '" << _shaderName << "'." << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned OpenGLShader::CompileShaderSource(unsigned type, const std::string& source) const
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

bool OpenGLShader::Bind() const
{
    if (IsOK())
    {
        glUseProgram(_id);
        return true;
    }
    return false;
}

void OpenGLShader::Unbind()
{
    glUseProgram(0);
}

void OpenGLShader::SetUniform1i(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform1iv(const std::string& name, const std::vector<int>& values) const
{
    glUniform1iv(GetUniformLocation(name), static_cast<int>(values.size()), values.data());
}

void OpenGLShader::SetUniform1f(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) const
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void OpenGLShader::SetUniformVec3f(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(GetUniformLocation(name), 1, &value.x);
}

void OpenGLShader::SetUniformVec4f(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(GetUniformLocation(name), 1, &value.x);
}

void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0].x);
}

int OpenGLShader::GetUniformLocation(const std::string& name) const
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

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(_id);
}
