/**
 * Grafik
 * File
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once

#include <fstream>
#include <format>
#include <optional>
#include <vector>


class File
{
public:
    File(const char* inFilePath) : filePath { inFilePath } {}
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    ~File() = default;
    
private:
    std::ifstream fin;
    std::string filePath;
    
public:    
    std::optional<std::string> Read();
    std::optional<std::vector<char>> ReadBytes();
};
