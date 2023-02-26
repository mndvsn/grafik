/**
 * Grafik
 * Vulkan: Pipeline
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include <glm/fwd.hpp>


class Pipeline
{
    bool _compiled { false };
    std::string _shaderName { };
    std::string _vertexFilePath { };
    std::string _fragmentFilePath { };

public:
    Pipeline(const std::string& vertexFile, const std::string& fragmentFile);
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    void operator=(const Pipeline&) = delete;
private:
    [[nodiscard]] std::string ExtractName(const std::string& filePath) const;
};
