/**
 * Grafik
 * File
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "gpch.h"
#include "File.h"

#include <format>
#include <mutex>
#include <stdexcept>


std::optional<std::string> File::Read()
{
    static std::mutex mutex;
    std::lock_guard lock(mutex);
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
        fin.close();
        return {};
    }

    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string contents = buffer.str();
    fin.close();

    if (contents.empty())
    {
        std::cout << "Warning: File " << filePath << " is empty!" << std::endl;
        return {};
    }

    return contents;
}

std::optional<std::vector<char>> File::ReadBytes()
{
    static std::mutex mutex;
    std::lock_guard lock(mutex);
    fin.open(filePath, std::ios::ate | std::ios::binary);

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
        fin.close();
        return {};
    }

    size_t fileSize = fin.tellg();
    std::vector<char> buffer(fileSize);

    fin.seekg(0);
    fin.read(buffer.data(), fileSize);
    fin.close();

    if (!fileSize)
    {
        std::cout << "Warning: File " << filePath << " is empty!" << std::endl;
        return {};
    }

    return buffer;
}