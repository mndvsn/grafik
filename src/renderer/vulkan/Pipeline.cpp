/**
 * Grafik
 * Vulkan: Pipeline
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "Pipeline.h"

#include "utils/File.h"

#include <cassert>
#include <filesystem>


Pipeline::Pipeline(RenderDevice& device, const std::string& vertexFile, const std::string& fragmentFile, const PipelineConfig& config)
    : _vertexFilePath { vertexFile }
    , _fragmentFilePath { fragmentFile }
    , _device { device }
    , _config { config }
{
    _shaderName = ExtractName(vertexFile);

    assert(_config.pipelineLayout != VK_NULL_HANDLE &&
        "Failed to create Pipeline; Missing PipelineLayout in config");
    assert(_config.renderPass != VK_NULL_HANDLE &&
        "Failed to create Pipeline; Missing RenderPass in config");
    
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

    std::cout << "Size vertex: " << vertexSource->size() << std::endl;
    std::cout << "Size fragment: " << fragmentSource->size() << std::endl;

    createShaderModule(*vertexSource, &_vertShaderModule);
    createShaderModule(*fragmentSource, &_fragShaderModule);

    VkPipelineShaderStageCreateInfo stages[2];
    // vertex stage
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = _vertShaderModule;
    stages[0].pName = "main";
    stages[0].flags = 0;
    stages[0].pNext = nullptr;
    stages[0].pSpecializationInfo = nullptr;
    // fragment stage
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = _fragShaderModule;
    stages[1].pName = "main";
    stages[1].flags = 0;
    stages[1].pNext = nullptr;
    stages[1].pSpecializationInfo = nullptr;

    // vertex buffer interpretation
    VkPipelineVertexInputStateCreateInfo vertexInputInfo { };
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;

    VkGraphicsPipelineCreateInfo pipelineInfo { };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &_config.inputAssemblyInfo;
    pipelineInfo.pViewportState = &_config.viewportInfo;
    pipelineInfo.pRasterizationState = &_config.rasterizationInfo;
    pipelineInfo.pMultisampleState = &_config.multisampleInfo;
    pipelineInfo.pColorBlendState = &_config.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &_config.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr; // used for setting line width/viewport dynamically

    pipelineInfo.layout = _config.pipelineLayout;
    pipelineInfo.renderPass = _config.renderPass;
    pipelineInfo.subpass = _config.subPass;

    // optimization
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(_device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error { "Failed to create Vulkan Graphics Pipeline!" };
    }
}

void Pipeline::createShaderModule(const std::vector<char>& source, VkShaderModule* module)
{
    VkShaderModuleCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    if (vkCreateShaderModule(_device.device(), &createInfo, nullptr, module) != VK_SUCCESS)
    {
        throw std::runtime_error { "Failed to create Vulkan Shader Module!" };
    }
}

std::string Pipeline::ExtractName(const std::string& filePath) const
{
    const std::filesystem::path path(filePath);
    return path.stem().string();
}

Pipeline::~Pipeline()
{
    vkDestroyShaderModule(_device.device(), _vertShaderModule, nullptr);
    vkDestroyShaderModule(_device.device(), _fragShaderModule, nullptr);
    vkDestroyPipeline(_device.device(), _graphicsPipeline, nullptr);
}
