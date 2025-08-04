#include "Renderer2D.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/OrthographicCamera.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Renderer/Texture.h"
#include <cstdint>

namespace BitEngine 
{
struct QuadVertex
{
    BMath::Vector3 Position;
    BMath::Vector4 Color;
    BMath::Vector3 TexCoords;
    float TextIndex;
};
struct Renderer2DData
{
    VertexArray* QuadVAO;
    VertexBuffer* QuadVBO;
    IndexBuffer* QuadIBO;
    Shader* Shader2D;
    Texture* WhiteTexture; 

};

static Renderer2DData s_RendererData;
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
        -0.3f,  0.3f,  0.0f, 1.0f,  // top-left     (0,1)
         0.3f,  0.3f,  1.0f, 1.0f,  // top-right    (1,1)
         0.3f, -0.3f,  1.0f, 0.0f,  // bottom-right (1,0)
        -0.3f, -0.3f,  0.0f, 0.0f   // bottom-left  (0,0)    
    };    
    unsigned int indices[] = 
    {
            0, 1, 2,
            0, 2, 3
    };
    s_RendererData.QuadVAO = VertexArray::Create();
    s_RendererData.QuadVAO->Bind();
    s_RendererData.QuadVBO = VertexBuffer::Create(vertices, sizeof(vertices));
    s_RendererData.QuadIBO = IndexBuffer::Create(indices, 6);
    BufferLayout QuadLayout = BufferLayout({
        { SHADER_DATA_TYPE::FLOAT2, "a_Position"}, 
        { SHADER_DATA_TYPE::FLOAT2, "a_TexCoords"}, 
    });
    s_RendererData.QuadVBO->SetBufferLayout(QuadLayout);
    s_RendererData.QuadVAO->AddVertexBuffer(s_RendererData.QuadVBO);
    s_RendererData.QuadVAO->SetIndexBuffer(s_RendererData.QuadIBO);

    s_RendererData.WhiteTexture = Texture::Create(1,1);
    uint8_t whitePixel[4] = { 255, 255, 255, 255 }; 
    s_RendererData.WhiteTexture->SetData(whitePixel, sizeof(whitePixel));

    s_RendererData.Shader2D = Shader::Create("assets/shaders/Shader2D.glsl");
    s_RendererData.Shader2D->Bind();
    s_RendererData.Shader2D->SetInt("u_TexImage", 0);
}
void Renderer2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
    m_RenderCommand->SetViewport(x, y, width, height);
}
void Renderer2D::SetClearColor(const BMath::Vector4& color) const 
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
void Renderer2D::DrawQuad(const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Vector4& color)
{
    s_RendererData.Shader2D->SetFloat4("u_Color",color);
    s_RendererData.WhiteTexture->Bind(0);
    s_RendererData.QuadVAO->Bind();
    m_RenderCommand->DrawIndexed(s_RendererData.QuadVAO);
}

void Renderer2D::DrawSprite(const BMath::Vector3& position, const BMath::Vector3& scale, Texture* texture)
{
    s_RendererData.Shader2D->SetFloat4("u_Color", {1.0f, 1.0f, 1.0f, 1.0f});
    texture->Bind(0);
    s_RendererData.QuadVAO->Bind();
    m_RenderCommand->DrawIndexed(s_RendererData.QuadVAO);
}
Renderer2D::~Renderer2D()
{
    delete s_RendererData.QuadVAO;
    delete s_RendererData.QuadVBO;
    delete s_RendererData.QuadIBO;
    delete s_RendererData.Shader2D;
    delete s_RendererData.WhiteTexture;
}
}
