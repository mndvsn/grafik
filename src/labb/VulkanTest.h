/**
 * Grafik
 * Lab: VulkanTest
 * Copyright 2023 Martin Furuberg
 */
#pragma once
#include "Lab.h"
#include "renderer/vulkan/VulkanModel.h"
#include "renderer/vulkan/VulkanPipeline.h"


class VulkanContext;

namespace labb
{
    struct SimplePushConstantData
    {
        glm::mat4 transform { 1.0f };
        alignas(16) glm::vec3 color { };
    };
    
    class LVulkanTest : public LLab
    {
    public:
        LVulkanTest();
        ~LVulkanTest() override;
       
        void OnTick(TickEvent& e) override;
        void OnRender(RenderEvent& e) override;
        void OnUI(UIEvent& e) override;
        
    private:
        void CreatePipeline();
        void CreateModel();

        VulkanContext* _context { };

        vk::PipelineLayout _pipelineLayout { };
        std::unique_ptr<VulkanPipeline> _pipeline { };
        std::unique_ptr<VulkanModel> _model { };
    };
}
