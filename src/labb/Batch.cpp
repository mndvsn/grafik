/**
 * Grafik
 * Lab: Batch
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Batch.h"

#include "ElementBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


namespace labb
{
    LBatch::LBatch(Renderer& rr) : LLab { rr }
    {
        int width, height;
        (void)GetRenderer().GetFramebufferSize(width, height);

        GetRenderer().SetClearColor({ 0.7f, 0.9f, 0.8f });

        // Define layout
        VertexBufferLayout layout;
        layout.Push<glm::vec3>(1); // position attribute
        layout.Push<glm::vec4>(1); // color attribute
        layout.Push<glm::vec2>(1); // uv attribute
        layout.Push<float>(1); // texture id attribute

        // Add vertex buffer with attributes to VAO
        _vao.AddVertexBuffer(_vbo, layout);

        // Generate element/index buffer and bind to VAO
        uint32_t indices[indicesCount];
        uint32_t offset { 0 };
        for (size_t i = 0; i < indicesCount; i += 6)
        {
            indices[i+0] = offset + 0;
            indices[i+1] = offset + 1;
            indices[i+2] = offset + 2;
            
            indices[i+3] = offset + 2;
            indices[i+4] = offset + 3;
            indices[i+5] = offset + 0;

            offset += 4;
        }
        
        const ElementBuffer ebo(indices, indicesCount);
        _vao.AddElementBuffer(ebo);
        
        // Define matrices
        _projection = glm::perspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 15.0f);
        _view = glm::translate(_view, _cameraPosition);

        // Check shader
        if (_shader.Bind())
        {
            // Load textures and bind to texture unit
            _texture1.emplace("data/textures/metal_plates.png");
            _texture2.emplace("data/textures/ground_base.jpg");
            if (_texture1->Bind(0) && _texture2->Bind(1))
            {
                _shader.SetUniform1iv("u_Textures", { 0, 1 });
            }
        }

        // unbind state
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }

    void LBatch::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);

        if (_bSpin)
        {
            _cycle = fmod(_cycle + static_cast<double>(_speed) * DeltaTime, 360.0);
        }
        _model = glm::mat4(1.0f);
        _model = rotate(_model, static_cast<float>(_cycle * glm::radians(180.0)), glm::vec3(0.0f, 1.0f, 0.0f));

        _view = glm::translate(glm::mat4(1.0f), _cameraPosition);
        
        _mvp = _projection * _view * _model;
    }

    void LBatch::BeginRender()
    {
        GetRenderer().Clear();

        _draws = 0;
        
        if (!_shader.Bind())
        {
            RenderError("Shader error!");
            return;
        }

        if (!_texture1->Bind(0) || !_texture2->Bind(1))
        {
            RenderError("Failed to load texture!");
            return;
        }

        // Create vertex data
        Vertex vertices[verticesCount];
        const auto quad1 = MakeQuad(-0.5f, 0.5f, 1.0f, 1.0f, 0);
        const auto quad2 = MakeQuad(0.5f, 0.5f, 1.0f, 1.0f, 1);
        const auto quad3 = MakeQuad(-0.5f, -0.5f, 1.0f, 1.0f, 1);
        const auto quad4 = MakeQuad(0.5f, -0.5f, 1.0f, 1.0f, 0);
        memcpy(vertices + quad1.size() * 0, quad1.data(), quad1.size() * sizeof(Vertex));
        memcpy(vertices + quad1.size() * 1, quad2.data(), quad2.size() * sizeof(Vertex));
        memcpy(vertices + quad1.size() * 2, quad3.data(), quad3.size() * sizeof(Vertex));
        memcpy(vertices + quad1.size() * 3, quad4.data(), quad4.size() * sizeof(Vertex));
        
        _vbo.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        _shader.SetUniformMat4f("u_MVP", _mvp);

        _vao.Bind();
        GetRenderer().Render(_vao, _shader);
        _draws++;
    }

    void LBatch::BeginGUI(bool* bKeep)
    {
        LLab::BeginGUI(bKeep);

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
        
        ImGui::Begin("Settings", bKeep, flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SameLine(0, 20.0f);
        ImGui::Text("Draw calls: %d", _draws);
        ImGui::Separator();
        ImGui::DragFloat("=", &_speed, 0.005f, -2.0f, 2.0f, "%.3f");
        ImGui::SameLine();
        ImGui::Checkbox("Speed", &_bSpin);
        ImGui::SliderFloat("Camera X", &_cameraPosition.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Camera Y", &_cameraPosition.y, -5.0f, 5.0f);
        ImGui::SliderFloat("Camera Z", &_cameraPosition.z, -10.0f, -1.0f);
        ImGui::End();
    }

    std::array<Vertex, 4> LBatch::MakeQuad(float x, float y, float width /*= 1.0f*/, float height /*= 1.0f*/, float texId /*= 0.0f*/)
    {
        Vertex v1 {}, v2 {}, v3 {}, v4 {};

        v1.Position = { x-width*0.5f, y+height*0.5f,  0.0f };
        v2.Position = { x+width*0.5f, y+height*0.5f,  0.0f };
        v3.Position = { x+width*0.5f, y-height*0.5f,  0.0f };
        v4.Position = { x-width*0.5f, y-height*0.5f,  0.0f };

        v1.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        v2.Color = { 0.0f, 1.0f, 0.0f, 1.0f };
        v3.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
        v4.Color = { 1.0f, 1.0f, 0.0f, 1.0f };

        v1.TexCoords = { 0.0f, 1.0f };
        v2.TexCoords = { 1.0f, 1.0f };
        v3.TexCoords = { 1.0f, 0.0f };
        v4.TexCoords = { 0.0f, 0.0f };

        v1.TexId = texId;
        v2.TexId = texId;
        v3.TexId = texId;
        v4.TexId = texId;
        
        return { v1, v2, v3, v4 };
    }
}
