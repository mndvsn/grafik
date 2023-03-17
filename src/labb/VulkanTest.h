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

    struct ObjectTransform
    {
        glm::vec3 translation   { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation      { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale         { 1.0f, 1.0f, 1.0f };

        // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        [[nodiscard]] glm::mat4 mat4() const
        {
            const float cos1 = glm::cos(rotation.y);
            const float sin1 = glm::sin(rotation.y);
            const float cos2 = glm::cos(rotation.x);
            const float sin2 = glm::sin(rotation.x);
            const float cos3 = glm::cos(rotation.z);
            const float sin3 = glm::sin(rotation.z);
            
            return glm::mat4
            {
                {
                    scale.x * (cos1 * cos3 + sin1 * sin2 * sin3),
                    scale.x * (cos2 * sin3),
                    scale.x * (cos1 * sin2 * sin3 - cos3 * sin1),
                    0.0f,
                },
                {
                    scale.y * (cos3 * sin1 * sin2 - cos1 * sin3),
                    scale.y * (cos2 * cos3),
                    scale.y * (cos1 * cos3 * sin2 + sin1 * sin3),
                    0.0f,
                },
                {
                    scale.z * (cos2 * sin1),
                    scale.z * -sin2,
                    scale.z * (cos1 * cos2),
                    0.0f,
                },
                {
                    translation.x, translation.y, translation.z, 1.0f
                }
            };
        }
    };

    struct Triangle
    {
        ObjectTransform transform { };
        glm::vec3 color { };
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

        int _objCount { 8 };
        double _animCycle { };
        std::vector<Triangle> _triangles { };
    };
}
