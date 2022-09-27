/**
 * Grafik
 * Lab: Mirror
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "Mirror.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


namespace labb
{
    LMirror::LMirror(Renderer& rr) : LLab { rr }
    {
        int width, height;
        (void)GetRenderer().GetFramebufferSize(width, height);

        GetRenderer().SetClearColor({ 0.7f, 0.9f, 0.8f });

        //TODO: Use Indices
        
        // Data for triangle
        constexpr float vertices[]
        {
            // position           // color           // uv
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                                                    
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                                                    
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
                                                    
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
                                                    
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
                                                    
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,

            -1.0f, -1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             1.0f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f
        };

        // Create Vertex Array Object
        _vao.emplace();

        // Generate vertex buffer for static draw
        const VertexBuffer vbo(vertices, sizeof(vertices));
        
        // Define layout
        VertexBufferLayout layout;
        layout.Push<float>(3); // position attribute, 2 floats
        layout.Push<float>(3); // color attribute, 3 floats
        layout.Push<float>(2); // uv attribute, 2 floats

        // Add vertex buffer with attributes to VAO
        _vao->AddVertexBuffer(vbo, layout);

        // Define matrices
        _projection = glm::perspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 1.0f, 10.0f);
        // _view = glm::translate(_view, glm::vec3(0.0f, 0.0f, -1.5f));
        _view = glm::lookAt(glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Create basic shader
        _shader.emplace("data/shaders/mirror.vert", "data/shaders/mirror.frag");
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

    void LMirror::BeginUpdate(double DeltaTime)
    {
        LLab::BeginUpdate(DeltaTime);

        if (_bSpin)
        {
            _cycle = fmod(_cycle + static_cast<double>(_speed) * DeltaTime, 360.0);
        }
        _model = glm::mat4(1.0f);
        _model = rotate(_model, static_cast<float>(_cycle * glm::radians(180.0)), glm::vec3(0.0f, 0.0f, 1.0f));

        _view = glm::lookAt(_cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        
        _mvp = _projection * _view * _model;
    }

    void LMirror::BeginRender()
    {
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

        _shader->SetUniformMat4f("u_MVP", _mvp);
        _shader->SetUniform1f("u_ReflectDarken", 1.0f);
        _shader->SetUniform1f("u_ColorAlpha", _colorAlpha);
        _vao->Bind();

        // Draw cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_STENCIL_TEST); // Start stencil testing
        
        // Draw plane
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set all bits to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace bit value to 1, if dp+st test pass
        glStencilMask(0xFF); // Write to stencil buffer
        glDepthMask(false); // Ignore depth buffer
        glClear(GL_STENCIL_BUFFER_BIT); // Clear default value 0 in buffer
        
        glDrawArrays(GL_TRIANGLES, 36, 6);

        // Draw mirrored cube
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Set test to value == 1
        glStencilMask(0x00); // No draw in stencil buffer
        glDepthMask(true); // Write to depth buffer

        const glm::mat4 _modelTrans = translate(_model, glm::vec3(0, 0, -1));
        _model = glm::scale(_modelTrans, glm::vec3(1, 1, -1));
        _mvp = _projection * _view * _model;
        _shader->SetUniformMat4f("u_MVP", _mvp);
        _shader->SetUniform1f("u_ReflectDarken", 1-_reflectDarken);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisable(GL_STENCIL_TEST); // End stencil testing
    }

    void LMirror::BeginGUI(bool* bKeep)
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
        ImGui::Separator();
        ImGui::DragFloat("=", &_speed, 0.005f, -2.0f, 2.0f, "%.3f");
        ImGui::SameLine();
        ImGui::Checkbox("Speed", &_bSpin);
        ImGui::SliderFloat("Camera X", &_cameraPosition.x, 1.0f, 5.0f);
        ImGui::SliderFloat("Camera Y", &_cameraPosition.y, 1.0f, 5.0f);
        ImGui::SliderFloat("Camera Z", &_cameraPosition.z, 1.0f, 5.0f);
        ImGui::SliderFloat("Color Mix", &_colorAlpha, 0.0f, 1.0f);
        ImGui::SliderFloat("Refl.Dark", &_reflectDarken, 0.0f, 1.0f);
        ImGui::End();
    }
}
