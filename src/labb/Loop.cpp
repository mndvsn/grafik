/**
 * Grafik
 * Lab: Loop
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Loop.h"

#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <cmath>


namespace labb
{
    LLoop::LLoop()
    {
        int width, height;
        if (!Renderer::GetFramebufferSize(width, height))
        {
            std::cout << "Error reading framebuffer size" << std::endl;
        }

        RenderCommand::SetClearColor(_bgColor);

        // Make array of vertices
        auto vertices = MakeCylinder({ 0.0f, 0.0f }, 2.0f, 1.0f, loopSegments);

        const VertexBuffer vbo { vertices.data(), sizeof(Vertex) * loopVerticesCount };
        
        // Define layout
        VertexBufferLayout layout;
        layout.Push<glm::vec3>(1); // position attribute
        layout.Push<glm::vec4>(1); // color attribute
        layout.Push<glm::vec2>(1); // uv attribute
        layout.Push<float>(1); // texture id attribute
        
        // Add vertex buffer with attributes to VAO
        _vao.AddVertexBuffer(vbo, layout);

        // Generate element/index buffer and bind to VAO
        unsigned indices[loopIndicesCount];
        unsigned offset { 0 };
        for (size_t i = 0; i < loopIndicesCount; i += 6)
        {
            indices[i+0] = offset + 0;
            indices[i+1] = offset + 1;
            indices[i+2] = offset + 2;
            
            indices[i+3] = offset + 2;
            indices[i+4] = offset + 3;
            indices[i+5] = offset + 0;

            offset += 4;
        }
        
        const ElementBuffer ebo(indices, loopIndicesCount);
        _vao.AddElementBuffer(ebo);
        
        // Define matrices
        const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        constexpr float magnification = 2.0f;
        // _projection = glm::perspective(120.0f, aspectRatio, 0.1f, 100.0f); (void)magnification;
        _projection = glm::ortho(aspectRatio * -magnification, aspectRatio * magnification, -magnification, magnification, 0.1f, 25.0f);
        _view = glm::translate(_view, _cameraPosition);

        // Check shader
        if (_shader.Bind())
        {
            // Bind texture units
            if (_texture0.Bind(0) && _texture1.Bind(1) && _texture2.Bind(2))
            {
                _shader.SetUniform1iv("u_Textures", { 0, 1, 2 });
            }
        }

        // unbind state
        Shader::Unbind();
        VertexArray::Unbind();
        VertexBuffer::Unbind();
    }

    void LLoop::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);

        if (_bSpin)
        {
            _cycle = fmod(_cycle + static_cast<double>(_speed) * DeltaTime, 360.0);
        }
        _model = glm::mat4(1.0f);
        _model = rotate(_model, glm::radians(_modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        _model = rotate(_model, glm::radians(_modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        _model = rotate(_model, static_cast<float>(_cycle * glm::radians(180.0)), glm::vec3(0.0f, 1.0f, 0.0f));

        _view = glm::lookAt(_cameraPosition, glm::vec3( 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f));
        
        _mvp = _projection * _view * _model;
    }

    void LLoop::BeginRender()
    {
        RenderCommand::SetClearColor(_bgColor);
        RenderCommand::ClearBuffer();

        if (!_shader.Bind())
        {
            RenderError("Shader error!");
            return;
        }

        if (!_texture0.Bind(0) || !_texture1.Bind(1) || !_texture2.Bind(2))
        {
            RenderError("Failed to load texture!");
            return;
        }

        _shader.SetUniformMat4f("u_MVP", _mvp);
        _shader.SetUniform1i("u_TexId", _texId);
        _shader.SetUniformVec4f("u_Color", _fgColor);

        _vao.Bind();
        // Z-sorting fix: Use either culling + draw 2x or disable depth test
        glEnable(GL_CULL_FACE);
        // glDepthMask(GL_FALSE);
        glCullFace(GL_BACK);
        Renderer::Render(_vao, _shader);
        glCullFace(GL_FRONT);
        Renderer::Render(_vao, _shader);
        glDisable(GL_CULL_FACE);
        // glDepthMask(GL_TRUE);
    }

    void LLoop::BeginGUI(bool* bKeep)
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
        
        ImGui::Begin("Loop", bKeep, flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
            static_cast<double>(ImGui::GetIO().Framerate));
        ImGui::Separator();
        ImGui::DragFloat("=", &_speed, 0.005f, -2.0f, 2.0f, "%.3f");
        ImGui::SameLine();
        ImGui::Checkbox("Speed", &_bSpin);
        ImGui::SliderFloat3("Rotation", &_modelRotation.x, 0.0f, 360.0f);
        ImGui::SliderFloat("Camera X", &_cameraPosition.x, -2.0f, 2.0f);
        ImGui::SliderFloat("Camera Y", &_cameraPosition.y, -2.0f, 2.0f);
        ImGui::SliderFloat("Camera Z", &_cameraPosition.z, -10.0f, -1.0f);
        ImGui::SliderInt("Texture", &_texId, 0, 2);
        ImGui::ColorEdit4("Foreground", &_fgColor.r);
        ImGui::ColorEdit3("Background", &_bgColor.r);
        ImGui::End();
    }

    std::array<Vertex, loopVerticesCount> LLoop::MakeCylinder(glm::vec2 origin, float radius, float length, int segments)
    {
        std::array<Vertex, loopVerticesCount> vertices {};
        Vertex* current = vertices.data();

        const float segStep { 360.0f / static_cast<float>(segments) };
        float degree { 0.0f };

        for (int i = 0; i < segments; i++)
        {
            const float curX { origin.x + sin(glm::radians(degree)) * radius };
            const float nextX { origin.x + sin(glm::radians(degree + segStep)) * radius };
            const float curZ { origin.y + cos(glm::radians(degree)) * radius };
            const float nextZ { origin.y + cos(glm::radians(degree + segStep)) * radius };
            
            // v1
            current->Position = { curX, length * 0.5f, curZ };
            current->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            current->TexCoords = { degree/180.0f, 1.0f };
            current->TexId = 0.0f;
            current++;

            // v2
            current->Position = { nextX, length * 0.5f, nextZ };
            current->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            current->TexCoords = { (degree+segStep)/180.0f, 1.0f };
            current->TexId = 0.0f;
            current++;

            // v3
            current->Position = { nextX, length * -0.5f, nextZ };
            current->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            current->TexCoords = { (degree+segStep)/180.0f, 0.0f };
            current->TexId = 0.0f;
            current++;

            // v4
            current->Position = { curX, length * -0.5f, curZ };
            current->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            current->TexCoords = { degree/180.0f, 0.0f };
            current->TexId = 0.0f;
            current++;

            degree += segStep;
        }

        return vertices;
    }
}
