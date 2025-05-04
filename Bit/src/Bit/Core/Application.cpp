#include "Application.h"
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Window.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Renderer/VertexArray.h"
#include <cstddef>


namespace BitEngine
{
    
Application::Application()
{
    m_EngineComponents = nullptr;    
    VAO = nullptr;
    VBO = nullptr;
    IBO = nullptr;
    shader = nullptr;
    texture = nullptr;
    m_RendererAPI = nullptr;
}
Application::~Application()
{
    delete VAO;
    delete VBO;
    delete IBO;
    delete shader;
    delete texture;
    delete m_RendererAPI;

}
void Application::InitializeEngineSystems(EngineComponents* engineComponents)
{
    m_EngineComponents = engineComponents;
    
}
void Application::OnInit()
{
    m_RendererAPI = RendererAPI::Create();
    m_RendererAPI->SetViewport(0, 0, m_EngineComponents->Window->GetWidth(), m_EngineComponents->Window->GetHeight());

    BIT_CORE_INFO("Window Width: {}", m_EngineComponents->Window->GetWidth());
    BIT_CORE_INFO("Window Height: {}", m_EngineComponents->Window->GetHeight());

//default logic goes before OnInit
    float vertices[] = {
         -0.5f, 0.5f, 0.0f,  0.0f, // uv top-left is (0, 0) 
         0.5f,  0.5f, 1.0f,  0.0f,
         0.5f, -0.5f, 1.0f,  1.0f,    // Position (x,y), UV (u,v)
        -0.5f, -0.5f, 0.0f,  1.0f,
    };    
    unsigned int indices[] = 
    {
            0, 1, 2,
            0, 2, 3
    };
    VAO = VertexArray::Create();
    VAO->Bind();
    std::cout << "Sizeof(Vertices) : " << sizeof(vertices) << "\n";
    VBO = VertexBuffer::Create(vertices, sizeof(vertices));
    IBO = IndexBuffer::Create(indices, 6);

    BufferLayout bufferLayout = BufferLayout({
        { SHADER_DATA_TYPE::FLOAT2, "a_Position"}, // Buffer Element is constructed as temp rvalue and passed to bufferlayout via std::move
        { SHADER_DATA_TYPE::FLOAT2, "a_TextureCoords"}
    });
    VBO->SetBufferLayout(bufferLayout);
    VAO->AddVertexBuffer(VBO);
    VAO->SetIndexBuffer(IBO);
    shader = Shader::Create("assets/shaders/BasicTexture.glsl");
    shader->Bind();
    texture = Texture::Create("assets/textures/daddy.bmp");
    texture->Bind(0);
    shader->SetInt("u_TextureImage", 0);
    BIT_CORE_ERROR("Hello {}", 13.4f);
}
void Application::OnRender()
{
    m_RendererAPI->SetClearColor(BitMath::Vector4(0.0f, 6.0f, 0.0f, 0.0f));
    m_RendererAPI->Clear();
    shader->Bind();
    m_RendererAPI->DrawIndexed(VAO);
}
void Application::OnUpdate(float deltaTime)
{
}

void Application::Run()
{
    m_IsRunning = true;
    OnInit();

    while(m_IsRunning)
    {
        OnUpdate(m_Time.GetDeltaTime());

        OnRender();

        m_EngineComponents->Window->OnUpdate();

    }
}




    
}
