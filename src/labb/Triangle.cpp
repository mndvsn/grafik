/**
 * Grafik
 * Lab: Triangle
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Triangle.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


namespace labb
{
    LTriangle::LTriangle(Renderer& rr) : LLab { rr }
    {
        int width, height;
        (void)GetRenderer().GetFramebufferSize(width, height);
        
        // Data for triangle
        constexpr float vertices[]
        {
            // position     // color
            0.0f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,
           -0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f
        };

        constexpr unsigned indices[]
        {
            0, 1, 2
        };
        
        // Create Vertex Array Object
        if (!_vao) _vao.emplace();
    
        // Generate vertex buffer for static draw
        const VertexBuffer vbo(vertices, sizeof(vertices));
        
        // Define layout
        VertexBufferLayout layout;
        layout.Push<float>(2); // position attribute, 2 floats
        layout.Push<float>(4); // color attribute, 4 floats

        // Add vertex buffer with attributes to VAO
        _vao->AddVertexBuffer(vbo, layout);

        // Generate element/index buffer and bind to VAO
        const ElementBuffer ebo(indices, 3);
        _vao->AddElementBuffer(ebo);
        
        // Define matrices
        _projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
        _view = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
    
        // Create a simple vertex color shader
        _triangleShader.emplace("data/shaders/color.vert", "data/shaders/color.frag");
        if (!_triangleShader->Bind())
        {
            RenderError("Shader error!");
        }

        // unbind state
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }

    void LTriangle::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);

        // Update matrices with current rotation
        _model = rotate(glm::mat4(1.0f), _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        _model = rotate(_model, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        _model = rotate(_model, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        _view = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        _mvp = _projection * _view * _model;
    }

    void LTriangle::BeginRender()
    {
        GetRenderer().SetClearColor({ 0.6f, 0.6f, 0.6f });
        GetRenderer().Clear();

        // Draw the triangle
        if (_triangleShader->Bind())
        {
            _triangleShader->SetUniformMat4f("u_MVP", _mvp);
            GetRenderer().Render(*_vao, *_triangleShader);
        }
    }

    void LTriangle::BeginGUI(bool* bKeep)
    {
        LLab::BeginGUI(bKeep);

        // Create Settings window
        constexpr float padding { 15.f };
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
        const ImVec2 work_pos = viewport->WorkPos;
        const ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + work_size.x - padding;
        window_pos.y = work_pos.y + padding;
        window_pos_pivot.x = 1.0f;
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

        // Add controls for rotation
        ImGui::Begin("Settings", bKeep, window_flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::SliderAngle("Rot X", &_rotation.x, -180.0f, 180.0f);
        ImGui::SliderAngle("Rot Y", &_rotation.y, -180.0f, 180.0f);
        ImGui::SliderAngle("Rot Z", &_rotation.z, -180.0f, 180.0f);
        ImGui::End();
    }
}
