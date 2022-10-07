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

#include <chrono>
#include <random>
#include <vector>


namespace labb
{
    LBatch::LBatch(Renderer& rr) : LLab { rr }
    {
        int width, height;
        (void)GetRenderer().GetFramebufferSize(width, height);

        GetRenderer().SetClearColor({ 1.0f, 1.0f, 1.0f });

        RandomizeSeed();

        // Define layout
        VertexBufferLayout layout;
        layout.Push<glm::vec3>(1); // position attribute
        layout.Push<glm::vec4>(1); // color attribute
        layout.Push<glm::vec2>(1); // uv attribute
        layout.Push<float>(1); // texture id attribute

        // Add vertex buffer with attributes to VAO
        _vao.AddVertexBuffer(_vbo, layout);

        // Make array of vertices
        _vertices = new Vertex[verticesCount];

        // Generate element/index buffer and bind to VAO
        unsigned indices[indicesCount];
        unsigned offset { 0 };
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
            _texture0.emplace(true);
            _texture1.emplace("data/textures/metal_plates.png");
            _texture2.emplace("data/textures/ground_base.jpg");
            if (_texture0->Bind(0) && _texture1->Bind(1) && _texture2->Bind(2))
            {
                _shader.SetUniform1iv("u_Textures", { 0, 1, 2 });
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

        if (!_texture0->Bind(0) || !_texture1->Bind(1) || !_texture2->Bind(2))
        {
            RenderError("Failed to load texture!");
            return;
        }

        // Fill buffer with vertex data
        Vertex* vertexPtr = _vertices;
        const size_t rows { static_cast<unsigned>(floor(sqrt(std::max(_quads, 1)))) };
        size_t cols { static_cast<unsigned>(ceil(_quads/rows)) };
        if (rows * cols != verticesCount)
        {
            cols++;
        }

        // Calc values for grid
        constexpr float size { 0.1f };
        const float startX { -size * static_cast<float>(cols) * 0.5f };
        const float startY {  size * static_cast<float>(rows) * 0.5f };

        randomEngine.seed(_seed);
        randomizer.reset();
        auto randomTextureId = std::bind(std::ref(randomizer), std::ref(randomEngine));

        size_t n{0}, curY{0}, curX{0};
        while (n < static_cast<size_t>(_quads))
        {
            const float texId = static_cast<float>(randomTextureId());
            const float x = startX + static_cast<float>(curX) * size + size * 0.5f;
            const float y = startY - static_cast<float>(curY) * size - size * 0.5f;
            const float z = texId * _breakAmount - _breakAmount;
            glm::vec4 color { 1.0f };
            color.r = static_cast<float>(curY) / static_cast<float>(rows);
            color.g = 1.0f - static_cast<float>(curX) / static_cast<float>(cols);
            color.b = static_cast<float>(curX) / static_cast<float>(cols);

            vertexPtr = MakeQuad(vertexPtr, x, y, z, size, size, texId, color);

            curY++;
            if (curY == rows)
            {
                curY = 0;
                curX++;
            }
            n++;
        }

        _vbo.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesCount * sizeof(Vertex), _vertices);

        _shader.SetUniformMat4f("u_MVP", _mvp);

        _vao.Bind();
        GetRenderer().Render(_vao, _shader, 0, _quads * 6 - 1);
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
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
            static_cast<double>(ImGui::GetIO().Framerate));
        ImGui::Text("Quads: %d", _quads);
        ImGui::SameLine(0, 20.0f);
        ImGui::Text("Draw calls: %d", _draws);
        ImGui::Separator();
        ImGui::DragFloat("=", &_speed, 0.005f, -2.0f, 2.0f, "%.3f");
        ImGui::SameLine();
        ImGui::Checkbox("Speed", &_bSpin);
        ImGui::SliderFloat("Camera X", &_cameraPosition.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Camera Y", &_cameraPosition.y, -5.0f, 5.0f);
        ImGui::SliderFloat("Camera Z", &_cameraPosition.z, -10.0f, -1.0f);
        ImGui::SliderFloat("Break-up", &_breakAmount, 0.0f, 3.0f);
        ImGui::DragInt("Quads", &_quads, 1, 0, batchQuadCapacity, "%d",
            ImGuiSliderFlags_Logarithmic);
        if (ImGui::Button("Randomize"))
        {
            RandomizeSeed();
        }
        ImGui::End();
    }

    Vertex* LBatch::MakeQuad(Vertex* vertexPtr, float x, float y, float z, float width /*= 1.0f*/, float height /*= 1.0f*/, float texId /*= 0.0f*/, glm::vec4 color /*1, 1, 1, 1*/)
    {
        vertexPtr->Position = { x-width*0.5f, y+height*0.5f, z };
        vertexPtr->Color = color;
        vertexPtr->TexCoords = { 0.0f, 1.0f };
        vertexPtr->TexId = texId;
        vertexPtr++;

        vertexPtr->Position = { x+width*0.5f, y+height*0.5f, z };
        vertexPtr->Color = color;
        vertexPtr->TexCoords = { 1.0f, 1.0f };
        vertexPtr->TexId = texId;
        vertexPtr++;

        vertexPtr->Position = { x+width*0.5f, y-height*0.5f, z };
        vertexPtr->Color = color;
        vertexPtr->TexCoords = { 1.0f, 0.0f };
        vertexPtr->TexId = texId;
        vertexPtr++;

        vertexPtr->Position = { x-width*0.5f, y-height*0.5f, z };
        vertexPtr->Color = color;
        vertexPtr->TexCoords = { 0.0f, 0.0f };
        vertexPtr->TexId = texId;
        vertexPtr++;

        return vertexPtr;
    }

    void LBatch::RandomizeSeed()
    {
        _seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    }

    LBatch::~LBatch()
    {
        delete[] _vertices;
    }
}
