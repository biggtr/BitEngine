#include "Renderer2D.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Scene/Compontents.h"
#include <cstdint>

namespace BitEngine 
{
struct QuadVertex
{
    BMath::vec3 Position;
    BMath::vec4 Color;
    BMath::vec2 TexCoords;
    // float TextIndex;
};
struct Renderer2DData
{
    static const uint32_t MaxQuads = 10000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;

    VertexArray* QuadVertexArray;
    VertexBuffer* QuadVertexBuffer;
    Shader* QuadShader;
    Texture* WhiteTexture; 

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

};

static Renderer2DData s_RenderData;
Renderer2D::Renderer2D()
{
    RendererAPI* rendererAPI = BitEngine::RendererAPI::Create();
    rendererAPI->SetAPI(RENDERER_API::OPENGL);
    m_RenderCommand = new RenderCommand();
    m_RenderCommand->Init(rendererAPI);
}
void Renderer2D::Init()
{

    s_RenderData.QuadVertexArray = VertexArray::Create();
    s_RenderData.QuadVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex));
    BufferLayout QuadLayout = BufferLayout({
            { SHADER_DATA_TYPE::FLOAT3, "a_Position"}, 
            { SHADER_DATA_TYPE::FLOAT4, "a_Color"}, 
            { SHADER_DATA_TYPE::FLOAT2, "a_TexCoords"}, 
            });
    s_RenderData.QuadVertexBuffer->SetBufferLayout(QuadLayout);
    s_RenderData.QuadVertexArray->AddVertexBuffer(s_RenderData.QuadVertexBuffer);
    s_RenderData.QuadVertexBufferBase = new QuadVertex[s_RenderData.MaxVertices];
    uint32_t* quadIndices = new uint32_t[s_RenderData.MaxIndices];
    uint32_t offset = 0;
    for(uint32_t i = 0; i < s_RenderData.MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;
        offset += 4;
    }
    IndexBuffer* QuadIBO = IndexBuffer::Create(quadIndices, s_RenderData.MaxIndices);
    s_RenderData.QuadVertexArray->SetIndexBuffer(QuadIBO);
    delete[] quadIndices;

    s_RenderData.WhiteTexture = Texture::Create(1,1);
    uint8_t whitePixel[4] = { 255, 255, 255, 255 }; 
    s_RenderData.WhiteTexture->SetData(whitePixel, sizeof(whitePixel));

    s_RenderData.QuadShader = Shader::Create("assets/shaders/QuadShader.glsl");
    s_RenderData.QuadShader->Bind();
    s_RenderData.QuadShader->SetInt("u_TexImage", 0);
}
void Renderer2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
    m_RenderCommand->SetViewport(x, y, width, height);
}
void Renderer2D::SetClearColor(const BMath::vec4& color) const 
{
    m_RenderCommand->SetClearColor(color);
}
void Renderer2D::Clear() const 
{
    m_RenderCommand->Clear();
}

void Renderer2D::BeginScene(CCamera* camera2D) 
{
    BIT_LOG_DEBUG("=== COMPLETE PROJECTION MATRIX ===");
    for(int i = 0; i < 16; i++) {
        BIT_LOG_DEBUG("P[%d] = %.6f", i, camera2D->ProjectionMatrix.Data[i]);
    }
    BIT_LOG_DEBUG("=== COMPLETE VIEW MATRIX ===");  
    for(int i = 0; i < 16; i++) {
        BIT_LOG_DEBUG("V[%d] = %.6f", i, camera2D->ViewMatrix.Data[i]);
    } 
    BMath::Matrix4x4 viewProjectionMatrix = camera2D->ProjectionMatrix * camera2D->ViewMatrix; 
    BIT_LOG_DEBUG("=== COMPLETE view PROJECTION MATRIX ===");
    for(int i = 0; i < 16; i++) {
        BIT_LOG_DEBUG("P[%d] = %.6f", i, viewProjectionMatrix.Data[i]);
    }
    s_RenderData.QuadShader->Bind();
    s_RenderData.QuadShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    BIT_LOG_DEBUG("BeginScene called");
    StartBatch();}
void Renderer2D::EndScene()
{
    BIT_LOG_DEBUG("EndScene called");
    Flush();
}

void Renderer2D::StartBatch()
{
    BIT_LOG_DEBUG("StartBatch called - resetting QuadIndexCount from %d to 0", s_RenderData.QuadIndexCount); 
    s_RenderData.QuadIndexCount = 0;
    s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;

}
void Renderer2D::Flush()
{
    if(s_RenderData.QuadIndexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase);
        s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);
        s_RenderData.QuadShader->Bind();
        m_RenderCommand->DrawIndexed(s_RenderData.QuadVertexArray, s_RenderData.QuadIndexCount);
    }

}
void Renderer2D::DrawQuad(const BMath::vec3& position, const BMath::vec3& scale, const BMath::vec4& color)
{
    BIT_LOG_DEBUG("Drawing quad at position: (%.2f, %.2f, %.2f), scale: (%.2f, %.2f, %.2f)", 
    position.x, position.y, position.z, scale.x, scale.y, scale.z);
    s_RenderData.QuadVertexBufferPtr->Position = position;
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadIndexCount += 6;

}
void Renderer2D::DrawQuad(const BMath::vec3& position, const BMath::vec3& scale, const BMath::vec4& color, Texture* texture)
{
}

Renderer2D::~Renderer2D()
{
    delete s_RenderData.QuadVertexArray;
    delete s_RenderData.QuadVertexBuffer;
    delete s_RenderData.QuadVertexBuffer;
    delete s_RenderData.QuadShader;
    delete s_RenderData.WhiteTexture;
    delete s_RenderData.QuadVertexBufferBase;
}
}
