/**
 * Grafik
 * Lab: Stacks
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Stacks.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <numbers>


namespace labb
{
    LStacks::LStacks(Renderer& rr) : LLab { rr }
    {
        int width, height;
        (void)GetRenderer().GetFramebufferSize(width, height);

        // Looks nicer without intersecting triangles 
        glDisable(GL_DEPTH_TEST);
        
        // Data for triangle
        constexpr float vertices[]
        {
            // position     // uv
            -0.5f, -0.5f,   0.0f, 0.0f,
             0.5f, -0.5f,   1.0f, 0.0f,
             0.5f,  0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f,   0.0f, 1.0f
        };

        constexpr unsigned indices[]
        {
            0, 1, 2,
            2, 3, 0
        };
        
        // Create Vertex Array Object
        _vao.emplace();

        // Generate vertex buffer for static draw
        const VertexBuffer vbo(vertices, sizeof(vertices));
        
        // Define layout
        VertexBufferLayout layout;
        layout.Push<float>(2); // position attribute, 2 floats
        layout.Push<float>(2); // uv attribute, 2 floats
        
        // Add vertex buffer with attributes to VAO
        _vao->AddVertexBuffer(vbo, layout);
        
        // Generate element/index buffer and bind to VAO
        const ElementBuffer ebo(indices, 6);
        _vao->AddElementBuffer(ebo);
        
        // Define matrices
        _projection = glm::perspective(65.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

        // Create basic shader
        _shader.emplace("data/shaders/basic.vert", "data/shaders/basic.frag");
        if (_shader->Bind())
        {
            // Load texture and bind to texture unit
            _texture.emplace("data/textures/metal_plates.png");
            if (constexpr int unit=0; _texture->Bind(unit))
            {
                _shader->SetUniform1i("u_Texture", unit);
            }
        }

        // unbind state
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }

    void LStacks::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);

        if (_bDoCycle)
        {
            _cycle = fmod(_cycle + static_cast<double>(_speed) * 50 * DeltaTime, 360.0);
        }
    }

    void LStacks::BeginRender()
    {
        GetRenderer().SetClearColor({ 0.0f, 0.0f, 0.0f });
        GetRenderer().Clear();

        if (!_shader->Bind())
        {
            RenderError("Shader error!");
            return;
        }

        if (!_texture->Bind())
        {
            RenderError("Failed to load texture!");
            return;
        }
        
        // Draw the quad (two triangles) a few times in a circle
        for (int i = 0; i < _count; i++)
        {
            const double degStep { 360 / static_cast<double>(_count) };
            const float deg { static_cast<float>(_cycle + degStep * static_cast<double>(i)) };
            const float rad { deg * std::numbers::pi_v<float> / 180.0f };
            glm::vec3 t {};
            t.x = _radius * sinf(rad);
            t.y = _radius * cosf(rad);
            t.z = static_cast<float>(i) * 0.02f - 1.0f;

            if (_bCycleColor)
            {
                _color.r = 0.5f + cos(rad) * 0.5f;
                _color.g = 0.5f + sin(rad) * 0.5f;
                _color.b = 0.5f + sin(rad + std::numbers::pi_v<float>) * 0.5f;
            }

            _model = translate(glm::mat4(1.0f), t);
            _model = rotate(_model, std::numbers::pi_v<float> * 0.2f * sin(rad), glm::vec3(0.0f, 1.0f, 0.0f));
            _model = rotate(_model, std::numbers::pi_v<float> * 2.0f * glm::fract(static_cast<float>(_cycle)*2.0f/360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            _model = scale(_model, glm::vec3(0.7f));
            _mvp = _projection * _view * _model;
            _shader->SetUniformMat4f("u_MVP", _mvp);
            _shader->SetUniformVec4f("u_Color", _color);
            GetRenderer().Render(*_vao, *_shader);
        }
    }

    void LStacks::BeginGUI(bool* bKeep)
    {
        LLab::BeginGUI(bKeep);

        // Create Settings window
        constexpr float padding { 15.f };
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
        const ImVec2 workPos = viewport->WorkPos;
        const ImVec2 workSize = viewport->WorkSize;
        ImVec2 position;
        position.x = workPos.x + workSize.x - padding;
        position.y = workPos.y + padding;
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::SetNextWindowPos(position, ImGuiCond_Always, { 1.0f, 0.0f });
        
        ImGui::Begin("Settings", bKeep, flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::SliderInt("Count", &_count, 1, 50);
        ImGui::SliderFloat("Radius", &_radius, -1.0f, 1.0f);
        ImGui::SliderFloat("Speed", &_speed, -1.0f, 5.0f);
        ImGui::Checkbox("Rotate", &_bDoCycle);
        ImGui::Checkbox("Cycle colors", &_bCycleColor);
        ImGui::End();
    }

    LStacks::~LStacks()
    {
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }
}
