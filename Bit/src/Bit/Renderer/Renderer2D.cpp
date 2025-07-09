#include "Renderer2D.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/VertexArray.h"

namespace BitEngine 
{

Renderer2D::Renderer2D()
{
    RendererAPI* rendererAPI = BitEngine::RendererAPI::Create();
    rendererAPI->SetAPI(RENDERER_API::OPENGL);
    m_RenderCommand = new RenderCommand();
    m_RenderCommand->Init(rendererAPI);
}
void Renderer2D::Init()
{

    float vertices[] = {
         -0.5f, 0.5f,  // uv top-left is (0, 0) 
         0.5f,  0.5f, 
         0.5f, -0.5f,     // Position (x,y), UV (u,v)
        -0.5f, -0.5f,
    };    
    unsigned int indices[] = 
    {
            0, 1, 2,
            0, 2, 3
    };
    m_QuadVAO = VertexArray::Create();
    m_QuadVAO->Bind();
    m_QuadVBO = VertexBuffer::Create(vertices, sizeof(vertices));
    m_QuadIBO = IndexBuffer::Create(indices, 6);
    BufferLayout QuadLayout = BufferLayout({
        { SHADER_DATA_TYPE::FLOAT2, "a_Position"}, // Buffer Element is constructed as temp rvalue and passed to bufferlayout via std::move
    });
    m_QuadVBO->SetBufferLayout(QuadLayout);
    m_QuadVAO->AddVertexBuffer(m_QuadVBO);
    m_QuadVAO->SetIndexBuffer(m_QuadIBO);
    m_QuadShader = Shader::Create("assets/shaders/quad_shader.glsl");
}
void Renderer2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
    m_RenderCommand->SetViewport(x, y, width, height);
}
void Renderer2D::SetClearColor(const BitMath::Vector4& color) const 
{
    m_RenderCommand->SetClearColor(color);
}
void Renderer2D::Clear() const 
{
    m_RenderCommand->Clear();
}

void Renderer2D::BeginScene()
{
}
void Renderer2D::DrawQuad(BitMath::Vector4 color)
{
    m_QuadVAO->Bind();
    m_QuadShader->SetFloat4("u_Color",color.X, color.Y, color.Z, color.W);
    m_QuadShader->Bind();    
    m_RenderCommand->DrawIndexed(m_QuadVAO);
}

Renderer2D::~Renderer2D()
{
    delete m_QuadVAO;
    delete m_QuadVBO;
    delete m_QuadIBO;
    delete m_QuadShader;
}
}
