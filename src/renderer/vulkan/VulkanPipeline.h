/**
 * Grafik
 * Vulkan: Pipeline
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "renderer/vulkan/VulkanDevice.h"


struct PipelineConfig
{
    vk::PipelineViewportStateCreateInfo viewportInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
    vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
    vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
    std::vector<vk::DynamicState> dynamicStateEnables;
    vk::PipelineLayout pipelineLayout { };
    vk::RenderPass renderPass { };
    uint32_t subPass { 0 };

    PipelineConfig()
    {
        // vertex data topology
        inputAssemblyInfo =
        {
            .topology                   = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable     = false
        };

        viewportInfo =
        {
            .viewportCount              = 1,
            .pViewports                 = nullptr,
            .scissorCount               = 1,
            .pScissors                  = nullptr,
        };

        // fragment stage
        rasterizationInfo =
        {
            .depthClampEnable           = false,
            .rasterizerDiscardEnable    = false,
            .polygonMode                = vk::PolygonMode::eFill,
            .cullMode                   = vk::CullModeFlagBits::eNone,
            .frontFace                  = vk::FrontFace::eClockwise,
            .depthBiasEnable            = false,
            .depthBiasConstantFactor    = 0.0f,
            .depthBiasClamp             = 0.0f,
            .depthBiasSlopeFactor       = 0.0f,
            .lineWidth                  = 1.0f
        };

        // anti-aliasing
        multisampleInfo =
        {
            .rasterizationSamples       = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable        = false,
            .minSampleShading           = 1.0f,
            .alphaToCoverageEnable      = false,
            .alphaToOneEnable           = false
        };

        // blending, combining colors in framebuffer
        colorBlendAttachment =
        {
            .blendEnable                = false,
            .srcColorBlendFactor        = vk::BlendFactor::eOne,
            .dstColorBlendFactor        = vk::BlendFactor::eZero,
            .colorBlendOp               = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor        = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor        = vk::BlendFactor::eZero,
            .alphaBlendOp               = vk::BlendOp::eAdd,
            .colorWriteMask             = vk::ColorComponentFlagBits::eR
                                        | vk::ColorComponentFlagBits::eG
                                        | vk::ColorComponentFlagBits::eB
                                        | vk::ColorComponentFlagBits::eA
        };

        colorBlendInfo =
        {
            .logicOpEnable              = false,
            .logicOp                    = vk::LogicOp::eCopy,
            .attachmentCount            = 1,
            .pAttachments               = &colorBlendAttachment,
            .blendConstants             = std::array { 0.0f, 0.0f, 0.0f, 0.0f }
        };

        depthStencilInfo =
        {
            .depthTestEnable            = true,
            .depthWriteEnable           = true,
            .depthCompareOp             = vk::CompareOp::eLess,
            .depthBoundsTestEnable      = false,
            .stencilTestEnable          = false,
            .front                      = { },
            .back                       = { },
            .minDepthBounds             = 0.0f,
            .maxDepthBounds             = 1.0f
        };

        dynamicStateEnables =
        {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
        };

        dynamicStateInfo =
        {
            .dynamicStateCount          = static_cast<uint32_t>(dynamicStateEnables.size()),
            .pDynamicStates             = dynamicStateEnables.data(),
        };
    }
};

class VulkanPipeline
{
    bool _compiled { false };
    std::string _shaderName { };
    std::string _vertexFilePath { };
    std::string _fragmentFilePath { };
    VulkanDevice& _device;
    PipelineConfig _config;

    vk::Pipeline _graphicsPipeline { };
    vk::ShaderModule _vertShaderModule { };
    vk::ShaderModule _fragShaderModule { };

public:
    VulkanPipeline(
        VulkanDevice& device,
        const std::string& vertexFile,
        const std::string& fragmentFile,
        const PipelineConfig& config);
    ~VulkanPipeline();

    VulkanPipeline(const VulkanPipeline&) = delete;
    VulkanPipeline& operator=(const VulkanPipeline&) = delete;

    void CreateShaderModule(const std::vector<char>& source, vk::ShaderModule& module) const;

    void Bind(const vk::CommandBuffer& commandBuffer) const;
    
    void Destroy();

private:
    [[nodiscard]] std::string ExtractName(const std::string& filePath) const;
};
