/**
 * Grafik
 * Vulkan: Model
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/vulkan/VulkanDevice.h"

#include <glm/glm.hpp>


class VulkanModel
{
public:
    struct Vertex
    {
        glm::vec2 position { };

        static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
    };
    
    VulkanModel(VulkanDevice& device, const std::vector<Vertex>& vertices);
    ~VulkanModel();

    VulkanModel(const VulkanModel&) = delete;
    VulkanModel& operator=(const VulkanModel&) = delete;

    void Bind(const vk::CommandBuffer& commandBuffer) const;
    void Draw(const vk::CommandBuffer& commandBuffer) const;

private:
    void CreateVertexBuffer(const std::vector<Vertex>& vertices);

    VulkanDevice& _device;
    vk::Buffer _vertexBuffer { };
    vk::DeviceMemory _vertexBufferMemory { };
    uint32_t _vertexCount { };
};
