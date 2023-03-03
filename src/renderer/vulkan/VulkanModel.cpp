/**
 * Grafik
 * Vulkan: Model
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanModel.h"

#include <cassert>


VulkanModel::VulkanModel(VulkanDevice& device, const std::vector<Vertex>& vertices)
    : _device { device }
{
    CreateVertexBuffer(vertices);
}

void VulkanModel::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
    _vertexCount = static_cast<uint32_t>(vertices.size());
    const vk::DeviceSize bufferSize = sizeof(Vertex) * _vertexCount;
    assert(_vertexCount >= 3 && "Vertex count must be at least 3 long");

    _device.CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        _vertexBuffer, _vertexBufferMemory);

    void* data;
    const auto result = _device.GetDevice().mapMemory(_vertexBufferMemory, 0, bufferSize, { }, &data);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to map memory" };
    }
    memcpy(data, vertices.data(), bufferSize);
    _device.GetDevice().unmapMemory(_vertexBufferMemory);
}

void VulkanModel::Bind(const vk::CommandBuffer& commandBuffer) const
{
    const vk::Buffer buffers[] { _vertexBuffer };
    constexpr vk::DeviceSize offsets[] { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);
}

void VulkanModel::Draw(const vk::CommandBuffer& commandBuffer) const
{
    commandBuffer.draw(_vertexCount, 1, 0, 0);
}

VulkanModel::~VulkanModel()
{
    _device.GetDevice().destroyBuffer(_vertexBuffer);
    _device.GetDevice().freeMemory(_vertexBufferMemory);
}

std::vector<vk::VertexInputBindingDescription> VulkanModel::Vertex::GetBindingDescriptions()
{
    return std::vector<vk::VertexInputBindingDescription>
    {
        {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = vk::VertexInputRate::eVertex,
        }
    };
}

std::vector<vk::VertexInputAttributeDescription> VulkanModel::Vertex::GetAttributeDescriptions()
{
    return std::vector<vk::VertexInputAttributeDescription>
    {
        // position
        {
            .location = 0,
            .binding = 0,
            .format = vk::Format::eR32G32B32Sfloat,
            .offset = 0,
        },
        // color
        {
            .location = 1,
            .binding = 0,
            .format = vk::Format::eR32G32B32Sfloat,
            .offset = offsetof(Vertex, color),
        }
    };
}
