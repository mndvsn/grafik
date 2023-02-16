/**
 * Grafik
 * OpenGL UI
 * Copyright 2023 Martin Furuberg 
 */
#include "gpch.h"
#include "OpenGLUI.h"

#include <glad/glad.h>

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


void OpenGLUI::Init(GLFWwindow* window)
{
    // Init ImGUI for GLFW, OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void OpenGLUI::Begin()
{
    // Begin ImGUI frame, reset
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    
    ImGui_ImplOpenGL3_NewFrame();
    
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();
}

void OpenGLUI::End()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

OpenGLUI::~OpenGLUI()
{
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
