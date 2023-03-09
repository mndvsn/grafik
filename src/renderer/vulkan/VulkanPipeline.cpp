/**
 * Grafik
 * Vulkan: Pipeline
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanPipeline.h"

#include "renderer/vulkan/VulkanModel.h"
#include "utils/File.h"

#include <cassert>
#include <filesystem>


VulkanPipeline::VulkanPipeline(VulkanDevice& device, const std::string& vertexFile, const std::string& fragmentFile, const PipelineConfig& config)
    : _vertexFilePath { vertexFile }
    , _fragmentFilePath { fragmentFile }
    , _device { device }
    , _config { config }
{
    _shaderName = ExtractName(vertexFile);

    assert(_config.pipelineLayout && "Failed to create Pipeline; Missing PipelineLayout in config");
    assert(_config.renderPass && "Failed to create Pipeline; Missing RenderPass in config");
    
    // Read vertex shader from file as binary
    File vsFile(vertexFile.c_str());
    const auto vertexSource = vsFile.ReadBytes();

    // Read fragment shader from file as binary
    File fsFile(fragmentFile.c_str());
    const auto fragmentSource = fsFile.ReadBytes();

    if (!vertexSource || !fragmentSource)
    {
        std::cout << "Error: Failed loading source for shader '" << _shaderName << "'." << std::endl;
        return;
    }

    Log::Debug("Pipeline <{0}> created (vertex: {1}, frag: {2})", _shaderName, vertexSource->size(), fragmentSource->size());

    CreateShaderModule(*vertexSource, _vertShaderModule);
    CreateShaderModule(*fragmentSource, _fragShaderModule);

    const vk::PipelineShaderStageCreateInfo stages[] =
    {
        {
            .stage = vk::ShaderStageFlagBits::eVertex,
            .module = _vertShaderModule,
            .pName = "main"
        },
        {
            .stage = vk::ShaderStageFlagBits::eFragment,
            .module = _fragShaderModule,
            .pName = "main"
        }
    };

    // vertex buffer interpretation
    auto bindings = VulkanModel::Vertex::GetBindingDescriptions();
    auto attributes = VulkanModel::Vertex::GetAttributeDescriptions();
    
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo
    {
        .vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
        .pVertexBindingDescriptions = bindings.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
        .pVertexAttributeDescriptions = attributes.data(),
    };

    vk::GraphicsPipelineCreateInfo pipelineInfo
    {
        .stageCount = 2,
        .pStages = stages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &_config.inputAssemblyInfo,
        .pViewportState = &_config.viewportInfo,
        .pRasterizationState = &_config.rasterizationInfo,
        .pMultisampleState = &_config.multisampleInfo,
        .pDepthStencilState = &_config.depthStencilInfo,
        .pColorBlendState = &_config.colorBlendInfo,

        // used for setting line width/viewport dynamically
        .pDynamicState = &_config.dynamicStateInfo,

        .layout = _config.pipelineLayout,
        .renderPass = _config.renderPass,
        .subpass = _config.subPass,

        // optimization
        .basePipelineHandle = nullptr,
        .basePipelineIndex = -1,
    };

    auto resultValue = _device.GetDevice().createGraphicsPipeline(nullptr, pipelineInfo);
    if (resultValue.result != vk::Result::eSuccess)
    {
        throw std::runtime_error { "Failed to create Vulkan Graphics Pipeline!" };
    }
    _graphicsPipeline = resultValue.value;
}

void VulkanPipeline::CreateShaderModule(const std::vector<char>& source, vk::ShaderModule& module) const
{
    const vk::ShaderModuleCreateInfo shaderModuleInfo
    {
        .codeSize = source.size(),
        .pCode = reinterpret_cast<const uint32_t*>(source.data()),
    };

    try
    {
        module = _device.GetDevice().createShaderModule(shaderModuleInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan Shader Module: " << error.what() << std::endl;
    }
}

void VulkanPipeline::Bind(const vk::CommandBuffer& commandBuffer) const
{
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphicsPipeline); // GRAPHICS, COMPUTE, RAY_TRACING
}

void VulkanPipeline::Destroy()
{
    const vk::Device& device = _device.GetDevice();
    device.destroyShaderModule(_vertShaderModule);
    device.destroyShaderModule(_fragShaderModule);
    device.destroyPipeline(_graphicsPipeline);
}

VulkanPipeline::~VulkanPipeline()
{
    Destroy();
}

std::string VulkanPipeline::ExtractName(const std::string& filePath) const
{
    const std::filesystem::path path(filePath);
    return path.stem().string();
}
