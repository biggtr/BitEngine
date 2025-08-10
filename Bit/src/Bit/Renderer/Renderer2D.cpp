#include "Renderer2D.h"
#include <array>
#include "Bit/Core/Logger.h"
#include "glad/glad.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Scene/Compontents.h"
#include <cstdint>

namespace BitEngine {
using std::array;

struct QuadVertex
{
    BMath::Vec3 Position;
    BMath::Vec4 Color;
    BMath::Vec2 TexCoords;
    f32 TexIndex;
};
struct Renderer2DData
{
    static const u32 MaxQuads = 10000;
    static const u32 MaxVertices = MaxQuads * 4;
    static const u32 MaxIndices = MaxQuads * 6;
    static const u32 MaxTextureSlots = 16;

    std::array<Texture*, MaxTextureSlots> TextureSlots;
    u32 TextureSlotIndex = 1; // starts from 1 because 0 is reserved for WhiteTexture

    VertexArray* QuadVertexArray;
    VertexBuffer* QuadVertexBuffer;
    Shader* QuadShader;
    Texture* WhiteTexture; 

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

};

static Renderer2DData s_RenderData;

b8 Renderer2D::Initialize()
{
    RendererAPI* rendererAPI = BitEngine::RendererAPI::Create();
    rendererAPI->SetAPI(RENDERER_API::OPENGL);
    m_RenderCommand = new RenderCommand();
    m_RenderCommand->Init(rendererAPI);

    s_RenderData.QuadVertexArray = VertexArray::Create();
    s_RenderData.QuadVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex));
    BufferLayout QuadLayout = BufferLayout({
            { SHADER_DATA_TYPE::FLOAT3, "a_Position"}, 
            { SHADER_DATA_TYPE::FLOAT4, "a_Color"}, 
            { SHADER_DATA_TYPE::FLOAT2, "a_TexCoords"}, 
            { SHADER_DATA_TYPE::FLOAT, "a_TexIndex"}, 
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
    i32 samplers[s_RenderData.MaxTextureSlots];
    for(u32 i = 0; i < s_RenderData.MaxTextureSlots; ++i)
    {
        samplers[i] = i;
    }
    s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;
    s_RenderData.QuadShader->SetIntArray("u_Textures", samplers, s_RenderData.MaxTextureSlots);
    return true;
}
void Renderer2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
    m_RenderCommand->SetViewport(x, y, width, height);
}
void Renderer2D::SetClearColor(const BMath::Vec4& color) const 
{
    m_RenderCommand->SetClearColor(color);
}
void Renderer2D::Clear() const 
{
    m_RenderCommand->Clear();
}

void Renderer2D::BeginScene(CCamera* camera2D) 
{
    int viewportWidth = static_cast<int>(camera2D->Right - camera2D->Left);
    int viewportHeight = static_cast<int>(camera2D->Top - camera2D->Bottom);
    glViewport(0, 0, viewportWidth, viewportHeight);
    BMath::Mat4 viewProjectionMatrix = camera2D->ProjectionMatrix * camera2D->ViewMatrix; 
    s_RenderData.QuadShader->Bind();
    s_RenderData.QuadShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    StartBatch();
}
void Renderer2D::EndScene()
{
    Flush();
}
void Renderer2D::StartBatch()
{
    s_RenderData.QuadIndexCount = 0;
    s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
    s_RenderData.TextureSlotIndex = 1;

}
void Renderer2D::Flush()
{
    
    if(s_RenderData.QuadIndexCount)
    {
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase);
        s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);

        for(u32 i = 0; i < s_RenderData.TextureSlotIndex; ++i)
        {
            s_RenderData.TextureSlots[i]->Bind(i);
        }
        s_RenderData.QuadShader->Bind();
        m_RenderCommand->DrawIndexed(s_RenderData.QuadVertexArray, s_RenderData.QuadIndexCount);
        
    }
    else
    {
        BIT_LOG_DEBUG("Nothing to flush - QuadIndexCount is 0");
    }
}void Renderer2D::DrawQuad(const BMath::Vec3& position, const BMath::Vec3& scale, const BMath::Vec4& color)
{
    f32 textureIndex = 0.0f;
    s_RenderData.QuadVertexBufferPtr->Position = position;
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadIndexCount += 6;

}
void Renderer2D::DrawQuad(const BMath::Vec3& position, const BMath::Vec3& scale, Texture* texture)
{
    float textureIndex = 0.0f;
    const BMath::Vec4 color(1.0f);

    //Check if we already have the texture stored inside the texture slots to be bound in future
    for(u32 i = 0; i < s_RenderData.TextureSlotIndex; ++i)
    {
        if(s_RenderData.TextureSlots[i]->GetID() == texture->GetID())
        {
            textureIndex = (f32)i;
            break;
        }
    }
    // if no texture found inside the texture slots we already have add it to be bound and used in future
    if(textureIndex == 0.0f)
    {
        textureIndex = s_RenderData.TextureSlotIndex;
        s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = texture;
        s_RenderData.TextureSlotIndex++;
    }

    s_RenderData.QuadVertexBufferPtr->Position = position;
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 0.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {1.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadVertexBufferPtr->Position = { position.x, position.y + scale.y, position.z };
    s_RenderData.QuadVertexBufferPtr->Color = color;
    s_RenderData.QuadVertexBufferPtr->TexCoords = {0.0f, 1.0f };
    s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_RenderData.QuadVertexBufferPtr++; 

    s_RenderData.QuadIndexCount += 6;
}

void Renderer2D::Shutdown()
{
    delete s_RenderData.QuadVertexArray;
    delete s_RenderData.QuadVertexBuffer;
    delete s_RenderData.QuadShader;
    delete s_RenderData.WhiteTexture;
    delete s_RenderData.QuadVertexBufferBase;
}
} // namespace BitEngine
