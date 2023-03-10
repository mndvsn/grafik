/**
 * Grafik
 * Lab: VulkanTest
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanTest.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/vulkan/VulkanContext.h"
#include "renderer/vulkan/VulkanSwapChain.h"
#include "renderer/vulkan/VulkanModel.h"

#include <imgui/imgui.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


namespace labb
{
    LVulkanTest::LVulkanTest()
    {
        _context = Application::Get().GetWindow()->GetContext<VulkanContext>().get();

        CreatePipeline();
        CreateModel();
    }

    void LVulkanTest::OnTick(TickEvent&)
    {
        
    }

    void LVulkanTest::OnRender(RenderEvent&)
    {
        const auto cmdBuffer = _context->GetCommandBuffer();
        
        // Bind command buffer to graphics binding point
        _pipeline->Bind(cmdBuffer);
        _model->Bind(cmdBuffer);

        for (int i = 0; i < 8; i++)
        {
            SimplePushConstantData push { };
            push.color = { static_cast<float>(i) * 0.1f, 0.0f, 0.0f };
            push.transform = glm::translate(push.transform, glm::vec3(-0.05f * static_cast<float>(i) , 0.0f, 0.0f));
            push.transform = glm::rotate(push.transform, static_cast<float>(i) * -0.05f * 3.14159265f, glm::vec3(0, 0, 1));
            cmdBuffer.pushConstants(_pipelineLayout,
                vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                0, sizeof(SimplePushConstantData), &push);
            _model->Draw(cmdBuffer);
        }
    }

    void LVulkanTest::OnUI(UIEvent& e)
    {
        LLab::OnUI(e);

        // Create Settings window
        constexpr float padding { 15.f };
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
        const ImVec2 workPos = viewport->WorkPos;
        const ImVec2 workSize = viewport->WorkSize;
        ImVec2 position;
        position.x = workPos.x + workSize.x - padding;
        position.y = workPos.y + padding;
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::SetNextWindowPos(position, ImGuiCond_Always, { 1.0f, 0.0f });
        
        ImGui::Begin("Vulkan", &_keepAlive, flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
            static_cast<double>(ImGui::GetIO().Framerate));
        ImGui::End();
    }

    void LVulkanTest::CreatePipeline()
    {
        vk::PushConstantRange pushConstantRange
        {
            .stageFlags                 = vk::ShaderStageFlagBits::eVertex
                                        | vk::ShaderStageFlagBits::eFragment,
            .offset                     = 0,
            .size                       = sizeof(SimplePushConstantData)
        };

        const vk::PipelineLayoutCreateInfo pipelineLayoutInfo
        {
            .setLayoutCount             = 0,
            .pushConstantRangeCount     = 1,
            .pPushConstantRanges        = &pushConstantRange,
        };
    
        try
        {
            _pipelineLayout = _context->GetDevice()->GetDevice().createPipelineLayout(pipelineLayoutInfo);
        }
        catch (vk::SystemError& error)
        {
            Log::Error("Failed to create Vulkan Pipeline Layout {}", error.what());
        }
    
        auto config = PipelineConfig { };
        config.renderPass = _context->GetSwapChain()->GetRenderPass();
        config.pipelineLayout = _pipelineLayout;

        _pipeline.reset(new VulkanPipeline {
            *_context->GetDevice(),
            "data/shaders/vktest.vert.spv",
            "data/shaders/vktest.frag.spv",
            config });
    }

    void LVulkanTest::CreateModel()
    {
        std::vector<VulkanModel::Vertex> vertices
        {
            { .position = {  0.0f, -0.5f, 0.0f }, .color = { 1.0f, 0.0f, 0.0f }},
            { .position = {  0.5f,  0.5f, 0.0f }, .color = { 0.0f, 1.0f, 0.0f }},
            { .position = { -0.5f,  0.5f, 0.0f }, .color = { 0.0f, 0.0f, 1.0f }}
        };
        
        _model = std::make_unique<VulkanModel>(*_context->GetDevice(), vertices);
    }

    LVulkanTest::~LVulkanTest()
    {
        _context->GetDevice()->GetDevice().waitIdle();        
        _context->GetDevice()->GetDevice().destroyPipelineLayout(_pipelineLayout);
        _pipeline.reset();
    }
}
