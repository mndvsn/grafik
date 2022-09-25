/**
 * Grafik
 * GLRender
 * Copyright 2012-2022 Martin Furuberg 
 */
#include "GLRender.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "utils/GLDebug.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// GLM
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include <cmath>
#include <numbers>
#include <sstream>


//#define DRAW_WIREFRAME

GLRender::GLRender(const char* title, const int width, const int height)
    : _title { title }, _width { width }, _height { height }
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup GLFW + GLSL
    std::stringstream glsl_version;
    glsl_version << "#version " << glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.str().c_str());
}

void GLRender::Setup()
{
    // Set blend function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Data for triangle
    constexpr float vertices[] =
    {
        // position     // uv
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,   0.0f, 1.0f
     };

    constexpr unsigned indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // Create Vertex Array Object
    if (!vao) vao.emplace();
    
    // Generate vertex buffer for static draw
    const VertexBuffer vbo(vertices, sizeof(vertices));

    // Define layout
    VertexBufferLayout layout;
    layout.Push<float>(2); // position attribute, 2 floats
    layout.Push<float>(2); // uv attribute, 2 floats

    // Add vertex buffer with attributes to VAO
    vao->AddVertexBuffer(vbo, layout);

    // Generate element/index buffer and bind to VAO
    const ElementBuffer ebo(indices, 6);
    vao->AddElementBuffer(ebo);

    // Define matrices    
    projection = glm::perspective(65.0f, static_cast<float>(_width) / static_cast<float>(_height), 0.1f, 100.0f);
    view = translate(glm::mat4(1.0f), glm::vec3(0.0f));
    
    // Create basic shader
    if (!basicShader) basicShader.emplace("src/res/shaders/basic.vert", "src/res/shaders/basic.frag");
    basicShader->Bind();

    // Load texture
    if (!texture) texture.emplace("src/res/textures/metal_plates.png");
    // bind to texture unit
    texture->Bind(0);
    basicShader->SetUniform1i("u_Texture", 0);

    // unbind state
    Shader::Unbind();
    VertexArray::Unbind();
    VertexBuffer::Unbind();
}

void GLRender::Run()
{
    double totalTimeElapsed { 0 }, timeElapsedNow { 0 }, deltaTime { 0 };
    double cycle {};

    // Values
    glm::vec4 color { 1.0f };
    bool bCycleColor { true };
    bool bDoCycle { true };
    int count { 25 };
    float radius { 0.5f };
    float speed { 0.7f };
    
    Renderer renderer;

#ifdef DRAW_WIREFRAME
    // Draw in wireframe mode
    renderer.SetWireframeMode(true);
#endif
    
    // Keep loop until the user closes the window
    while (!glfwWindowShouldClose(_window))
    {
        // Update timers
        timeElapsedNow = glfwGetTime();
        deltaTime = timeElapsedNow - totalTimeElapsed;
        totalTimeElapsed = timeElapsedNow;
        
        renderer.Clear();

        // Begin ImGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (bDoCycle)
        {
            cycle = fmod(cycle + static_cast<double>(speed) * 50 * deltaTime, 360.0);
        }

        // Draw the quad (two triangles) a few times in a circle
        basicShader->Bind();
        texture->Bind();
        
        for (int i = 0; i < count; i++)
        {
            const double degStep { 360 / static_cast<double>(count) };
            const float deg { static_cast<float>(cycle + degStep * static_cast<double>(i)) };
            const float rad { deg * std::numbers::pi_v<float> / 180.0f };
            glm::vec3 t {};
            t.x = radius * sinf(rad);
            t.y = radius * cosf(rad);
            t.z = static_cast<float>(i) * 0.02f - 1.0f;

            if (bCycleColor)
            {
                color.r = 0.5f + cos(rad) * 0.5f;
                color.g = 0.5f + sin(rad) * 0.5f;
                color.b = 0.5f + sin(rad + std::numbers::pi_v<float>) * 0.5f;
            }

            model = translate(glm::mat4(1.0f), t);
            model = rotate(model, std::numbers::pi_v<float> * 0.2f * sin(rad), glm::vec3(0.0f, 1.0f, 0.0f));
            model = rotate(model, std::numbers::pi_v<float> * 2.0f * glm::fract(static_cast<float>(cycle)*2.0f/360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = scale(model, glm::vec3(0.7f, 0.7f, 1.0f));
            mvp = projection * view * model;
            basicShader->SetUniformMat4f("u_MVP", mvp);
            basicShader->SetUniformVec4f("u_Color", color);
            renderer.Render(*vao, *basicShader);
        }

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
        
        ImGui::Begin("Settings", nullptr, window_flags);
        ImGui::Text("Render %.3f ms/f (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::SliderInt("Count", &count, 1, 50);
        ImGui::SliderFloat("Radius", &radius, -1.0f, 1.0f);
        ImGui::SliderFloat("Speed", &speed, -1.0f, 5.0f);
        // ImGui::SliderFloat2("1 XY", &translation.x, -1.0f, 1.0f);
        ImGui::Checkbox("Rotate", &bDoCycle);
        ImGui::Checkbox("Cycle colors", &bCycleColor);
        ImGui::End();
        
        // Render ImGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    // Clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
    glfwDestroyWindow(_window);
    glfwTerminate();
}
