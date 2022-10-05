/**
 * Grafik
 * GLRender
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "GLRender.h"

#include "Renderer.h"
#ifdef _DEBUG
#include "utils/GLDebug.h"
#endif

// Labb
#include "labb/ClearColor.h"
#include "labb/Mirror.h"
#include "labb/Stacks.h"
#include "labb/Triangle.h"
#include "labb/Batch.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <iostream>
#include <memory>
#include <sstream>


//#define DRAW_WIREFRAME

GLRender::GLRender(const char* title, const int width, const int height, const char* initLab)
    : _title { title }
    , _width { width }
    , _height { height }
    , _initLab { initLab }
{
    
}

void GLRender::Init()
{
    glfwSetErrorCallback(glfwError);
    if (!glfwInit())
    {
        {
            throw std::runtime_error("GLFW initialization failed!");
        }
    }

    // Set context to 4.6 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create window and init glfw with context
    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        {
            throw std::runtime_error("OpenGL context initialization failed!");
        }
    }
    glfwMakeContextCurrent(_window);

    // swap buffers in sync with screen freq aka v-sync
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        {
            throw std::runtime_error("GLAD initialization failed!");
        }
    }

#   ifdef _DEBUG
    InitDebug();
#   endif

    // Setup Dear ImGui context
    InitImGUI();
}

void GLRender::InitImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark(&style);
    style.WindowRounding = 3.0f;
    style.FrameRounding = 3.0f;

    // Setup GLFW + GLSL
    std::stringstream glsl_version;
    glsl_version << "#version " << glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.str().c_str());

    const auto font = io.Fonts->AddFontFromFileTTF("data/fonts/JetBrainsMonoNL-Light.ttf", 15.0f);
    IM_ASSERT(font != nullptr);
}

void GLRender::Setup()
{
    // Set blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLRender::Run()
{
    Renderer renderer(_window);
    
    double totalTimeElapsed { 0 },
        timeElapsedNow { 0 },
        deltaTime { 0 };

#ifdef DRAW_WIREFRAME
    // Draw in wireframe mode
    renderer.SetWireframeMode(true);
#endif

    // Set to false to exit active lab
    static bool bKeep { true };
    auto lab = std::unique_ptr<labb::LLab> {};
    const auto menu = std::make_unique<labb::LLabMenu>(renderer, lab);

    // Add labs to main menu
    menu->RegisterLab<labb::LClearColor>("Clear Color", "clearcolor");
    menu->RegisterLab<labb::LTriangle>("Triangle", "triangle");
    menu->RegisterLab<labb::LStacks>("Stacks", "stacks");
    menu->RegisterLab<labb::LMirror>("Mirror", "mirror");
    menu->RegisterLab<labb::LBatch>("Batch", "batch");

    // Create an initial lab if requested and matching shortname is found
    if (!_initLab.empty())
    {
        if (const auto maybeLab = menu->CreateLabIfExists(_initLab))
        {
            lab.reset(*maybeLab);
        }
    }

    // Keep running loop until we should shutdown
    while (!glfwWindowShouldClose(_window))
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);

        if (lab)
        {
            lab->BeginUpdate(deltaTime);
            lab->BeginRender();
        }
        else
        {
            menu->BeginRender();
        }

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        
        // Begin ImGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow();

        // Draw main menu UI
        menu->BeginGUI(&bKeep);

        if (lab)
        {
            // Draw lab specific UI
            lab->BeginGUI(&bKeep);
            if (!bKeep)
            {
                lab.reset();
                bKeep = true;
            }
        }
        else
        {
            // Draw selection window
            menu->BeginBigMenu();
        }

        // Render ImGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

#ifdef _DEBUG
void GLRender::InitDebug()
{
    // Print adapter info
    std::cout << "Shading language: \t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "GL Version: \t\t"     << glGetString(GL_VERSION) << "\n";
    std::cout << "Vendor: \t\t"         << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: \t\t"       << glGetString(GL_RENDERER) << std::endl;

    // Set up debug/error message handling
    int flags {};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(HandleGLDebugMessage, nullptr);
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
    }
}
#endif

void GLRender::glfwError(int error, const char* description)
{
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}

GLRender::~GLRender()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(_window);
    glfwTerminate();
}
