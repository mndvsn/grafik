/**
 * Grafik
 * Shader
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "Shader.h"

#include "renderer/RendererAPI.h"
#include "renderer/opengl/OpenGLShader.h"

#include <filesystem>


std::shared_ptr<Shader> Shader::Create(const std::string& vertexFile, const std::string& fragmentFile)
{
    const std::string shaderName = ExtractName(vertexFile);
    
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:           return nullptr;
        case RendererAPI::API::OpenGL:         return std::make_shared<OpenGLShader>(shaderName, vertexFile, fragmentFile);
        case RendererAPI::API::Vulkan:         return nullptr;
    }
    return nullptr;
}

std::string Shader::ExtractName(const std::string& filePath)
{
    const std::filesystem::path path(filePath);
    return path.stem().string();
}

void Shader::Unbind()
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None:           break;
        case RendererAPI::API::OpenGL:         OpenGLShader::Unbind();
        case RendererAPI::API::Vulkan:         break;
    }
}
