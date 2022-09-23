/**
 * Grafik
 * File
 * Copyright 2012-2022 Martin Furuberg 
 */
#pragma once

#include <fstream>
#include <mutex>
#include <format>

class File
{
public:
    File(const char* inFilePath);
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    ~File() = default;
    
private:
    std::ifstream fin;
    std::string filePath;
    
public:    
    std::string Read();
};
