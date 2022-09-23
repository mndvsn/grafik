/**
 * Grafik
 * File
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "File.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

File::File(const char* inFilePath)
{
    filePath = inFilePath;
}

std::string File::Read()
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    fin.open(filePath, std::ios::in);

    try
    {
        if (!fin.is_open())
        {
            throw std::runtime_error(std::format("File: Unable to open file {}", filePath));
        }
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string contents = buffer.str();

    fin.close();
    
    return contents;
}