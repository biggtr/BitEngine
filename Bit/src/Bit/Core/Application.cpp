#include "Application.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Renderer/VertexArray.h"

namespace BitEngine
{
    
Application::Application()
{
    m_EngineComponents = nullptr;
}
Application::~Application()
{
    delete VAO;
    delete VBO;
    delete IBO;
    delete bufferLayout;
    delete shader;
    delete texture;

}
void Application::InitializeEngineSystems(EngineComponents* engineComponents)
{
    m_EngineComponents = engineComponents;
    
}
void Application::OnInit()
{
     glViewport(0, 0, 
        m_EngineComponents->Window.GetWidth(),
        m_EngineComponents->Window.GetHeight()
    );
//default logic goes before OnInit
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 1.0f,  // Position (x,y), UV (u,v)
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f
    };    
    unsigned int indices[] = 
    {
            0, 1, 2,
            0, 2, 3
    };

    VAO = new VertexArray();
    VBO = new VertexBuffer(vertices, sizeof(vertices));
    IBO = new IndexBuffer(indices, 6);

    bufferLayout = new BufferLayout({
        { SHADER_DATA_TYPE::FLOAT2, "a_Position"}, // Buffer Element is constructed as temp rvalue and passed to bufferlayout via std::move
        { SHADER_DATA_TYPE::FLOAT2, "a_TextureCoords"},
    });
    VBO->SetBufferLayout(bufferLayout);
    VAO->Bind();
    VAO->AddVertexBuffer(VBO);
    VAO->SetIndexBuffer(IBO);
    shader = new Shader("assets/shaders/BasicTexture.glsl");
    shader->Bind();
    texture = new Texture("assets/textures/Basic.bmp");
    texture->Bind(0);
    BIT_CORE_INFO("Hello from application init..");

    if(VAO->GetVertexBuffer()[0] != nullptr)
    {
        BIT_CORE_ERROR("VertexBuffer is not null");
    }

    if(VAO->GetIndexBuffer() != nullptr)
    {

        BIT_CORE_ERROR("IndexBuffer is not null");
    }
    for(auto element : bufferLayout->GetBufferElements())
    {
        std::cout << element.AttributeName << " " << element.Offset << " " << element.Size << " " <<  std::endl;
    }

}
void Application::OnRender()
{
    VAO->Bind();
    shader->Bind();
    const IndexBuffer* indexBuffer = VAO->GetIndexBuffer();
    indexBuffer->Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Application::OnUpdate(float deltaTime)
{
}

void Application::Run()
{
    m_IsRunning = true;
    OnInit();

    while(!m_EngineComponents->Window.ShouldClose() &&  m_IsRunning)
    {
        m_EngineComponents->Window.ProcessInput();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        OnUpdate(m_Time.GetDeltaTime());

        OnRender();

        m_EngineComponents->Window.OnUpdate();

    }
}




    
}
