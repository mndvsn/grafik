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

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include <cmath>
#include <numbers>


//#define DRAW_WIREFRAME

GLRender::GLRender(const char* title, const int width, const int height)
    : _title { title }, _width { width }, _height { height }
{
    
}

void GLRender::Init()
{
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

#ifdef _DEBUG
    // Print adapter info
    std::cout << "Shading language: \t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "GL Version: \t\t"     << glGetString(GL_VERSION) << "\n";
    std::cout << "Vendor: \t\t"         << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: \t\t"       << glGetString(GL_RENDERER) << std::endl;

    // Set up debug/error message handling
    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(HandleGLDebugMessage, nullptr);
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
    }
#endif
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
    const glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f);
    const glm::mat4 view = translate(glm::mat4(1.0f), glm::vec3(-0.2, 0, 0));
    const glm::mat4 model = translate(glm::mat4(1.0f), glm::vec3(0, 0.1, 0));
    mvp = projection * view * model;
    
    // Create basic shader
    if (!basicShader) basicShader.emplace("src/res/shaders/basic.vert", "src/res/shaders/basic.frag");
    basicShader->Bind();
    basicShader->SetUniformMat4f("u_MVP", mvp);
    basicShader->SetUniform4f("u_Color", 1.0f, 0.0f, 1.0f, 1.0f); // set initial color

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
       
        // Draw two triangles with vertex array object, compiled shader and set uniform 
        basicShader->Bind();

        cycle = fmod(cycle + 50*deltaTime, 360.0);
        const auto diffSin = static_cast<float>(sin(cycle * std::numbers::pi / 180.0));
        const auto diffCos = static_cast<float>(cos(cycle * std::numbers::pi / 180.0));
        basicShader->SetUniform4f("u_Color", 0.5f + diffCos*0.5f, 0.5f + diffSin*0.5f, 0.0f, 1.0f);

        texture->Bind();
        renderer.Render(*vao, *basicShader);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

GLRender::~GLRender()
{
    glfwTerminate();
}
