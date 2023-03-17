/**
 * Grafik
 * Lab: VulkanTest
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "VulkanTest.h"

#include "core/Application.h"
#include "components/Window.h"
#include "renderer/RenderCommand.h"
#include "renderer/vulkan/VulkanContext.h"
#include "renderer/vulkan/VulkanSwapChain.h"
#include "renderer/vulkan/VulkanModel.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>


namespace labb
{
    LVulkanTest::LVulkanTest()
    {
        _context = Application::Get().GetWindow()->GetContext<VulkanContext>().get();

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.12f });
        
        CreatePipeline();
        CreateModel();

        for (int i = 0; i < _objCount; i++)
        {
            const auto step = static_cast<float>(i);

            Triangle triangle
            {
                .color = {
                    step * 0.1f,
                    step * 0.1f,
                    step * 0.1f
                }
            };
            _triangles.emplace_back(triangle);
        }
    }

    void LVulkanTest::OnTick(TickEvent& e)
    {
        const auto delta = e.GetDeltaTime();
        _animCycle = glm::mod(_animCycle + 90.0 * delta, 360.0);
        const auto radCycle = glm::radians(_animCycle);

        for (double step = 0.0; auto& triangle : _triangles)
        {
            triangle.transform.translation.x = static_cast<float>(glm::sin(radCycle) * step * 0.04);
            triangle.transform.rotation.z = static_cast<float>(glm::radians(15 * glm::sin(radCycle)));
            step += 1.0;
        }
    }

    void LVulkanTest::OnRender(RenderEvent& e)
    {
        // Bind command buffer to graphics binding point
        const auto cmdBuffer = _context->GetCommandBuffer();
        _pipeline->Bind(cmdBuffer);
        _model->Bind(cmdBuffer);

        for (const auto& triangle : _triangles)
        {
            SimplePushConstantData push
            {
                .transform = triangle.transform.mat4(),
                .color = triangle.color
            };
            cmdBuffer.pushConstants(_pipelineLayout,
                vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                0, sizeof(SimplePushConstantData), &push);
            _model->Draw(cmdBuffer);
        }
        e.Handled();
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
        ImGui::Separator();
        ImGui::Text("Animation: %d %%", static_cast<int>(_animCycle / 360. * 100));
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
