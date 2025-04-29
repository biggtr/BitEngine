#include "Application.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Renderer/VertexArray.h"
#include <GL/gl.h>
#include <GL/glext.h>

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
    //default logic goes before OnInit
    float vertices[] = 
    {
            -0.5,   0.5, 0.0, 1.0, 
            -0.5,  -0.5, 0.0, 0.0, 
             0.5,  -0.5, 1.0, 0.0, 
             0.5,   0.5, 1.0, 1.0, 
    };
    float indices[] = 
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
    VBO->Bind();
    IBO->Bind();
    VAO->Bind();
    VAO->AddVertexBuffer(VBO);
    VAO->SetIndexBuffer(IBO);
    shader = new Shader("assets/shaders/BasicTexture.glsl");
    shader->Bind();
    texture = new Texture("assets/textures/Basic.bmp");
    texture->Bind(0);

}
void Application::OnRender()
{
    VAO->Bind();
    glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
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
