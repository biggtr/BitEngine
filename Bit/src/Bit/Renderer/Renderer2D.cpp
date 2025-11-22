#include "Renderer2D.h"
#include <array>
#include "Bit/Core/Logger.h"
#include "Bit/Resources/ShaderManager.h"
#include "box2d/collision.h"
#include "glad/glad.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/ECS/Compontents.h"
#include <cstdint>

namespace BitEngine 
{

static u32 GetGLRenderMode(RENDER_MODE renderMode) 
{
    switch (renderMode) 
    {
        case RENDER_FILL:
            return GL_FILL;
        case RENDER_LINE:
            return GL_LINE;
        default:
            return 0;
    }
}
struct QuadVertex
{
    BMath::Vec4 Position;
    BMath::Vec4 Color;
    BMath::Vec2 TexCoords;
    f32 TexIndex;
};
struct CircleVertex
{
    BMath::Vec4 WorldPosition;
    BMath::Vec3 NormalizedPosition; // -1, 1
    BMath::Vec4 Color;
    f32 Thickness;
    f32 Fade;
};
struct LineVertex
{

    BMath::Vec3 Position;
    BMath::Vec4 Color;
};

struct RenderStats
{
    u32 DrawCalls = 0;
    u32 QuadCount = 0;
};
static RenderStats Stats; 


struct Renderer2DData
{
    static const u32 MaxQuads = 10000;
    static const u32 MaxVertices = MaxQuads * 4;
    static const u32 MaxIndices = MaxQuads * 6;
    static const u32 MaxTextureSlots = 16;

    IndexBuffer* QuadIndexBuffer = nullptr;

    VertexArray* QuadVertexArray = nullptr;
    VertexBuffer* QuadVertexBuffer = nullptr;
    Shader* QuadShader = nullptr;
    Texture* WhiteTexture = nullptr; 

    VertexArray* CircleVertexArray = nullptr;
    VertexBuffer* CircleVertexBuffer = nullptr;
    Shader* CircleShader = nullptr;

    VertexArray* LineVertexArray = nullptr;
    VertexBuffer* LineVertexBuffer = nullptr;
    Shader* LineShader = nullptr;

    VertexArray* RectVertexArray = nullptr;
    VertexBuffer* RectVertexBuffer = nullptr;
    Shader* RectShader = nullptr;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    uint32_t CircleIndexCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    uint32_t LineVertexCount = 0;
    LineVertex* LineVertexBufferBase = nullptr;
    LineVertex* LineVertexBufferPtr = nullptr;

    float LineWidth = 2.0f;
    
    std::array<Texture*, MaxTextureSlots> TextureSlots{};
    u32 TextureSlotIndex = 1; // starts from 1 because 0 is reserved for WhiteTexture
                            
    BMath::Vec4 QuadVertexPositions[4]{};
};

static Renderer2DData s_RenderData;

b8 Renderer2D::Initialize()
{
    m_RenderCommand = new RenderCommand();
    if(!m_RenderCommand->Initialize())
    {
        BIT_LOG_ERROR("failed to Initialize RenderCommand");
        return false;
    }
    // TODO: Move this to application one manager per app to manage all shaders 
    // HACK
    m_ShaderManager = new ShaderManager();
    if(!m_ShaderManager)
    {
        BIT_LOG_ERROR("failed to Initialize ShaderManager");
        return false;
    }

    s_RenderData.QuadVertexArray = VertexArray::Create();
    s_RenderData.QuadVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex));
    BufferLayout QuadLayout = BufferLayout({
            { SHADER_DATA_TYPE::FLOAT4, "a_Position"}, 
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
    s_RenderData.QuadIndexBuffer= IndexBuffer::Create(quadIndices, s_RenderData.MaxIndices);
    s_RenderData.QuadVertexArray->SetIndexBuffer(s_RenderData.QuadIndexBuffer);
    delete[] quadIndices;

    s_RenderData.CircleVertexArray = VertexArray::Create();
    s_RenderData.CircleVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(CircleVertex));
    BufferLayout CircleLayout = BufferLayout({
            { SHADER_DATA_TYPE::FLOAT4, "a_WorldPosition"}, 
            { SHADER_DATA_TYPE::FLOAT3, "a_NormalizedPosition"}, 
            { SHADER_DATA_TYPE::FLOAT4, "a_Color"}, 
            { SHADER_DATA_TYPE::FLOAT, "a_Thickness"}, 
            { SHADER_DATA_TYPE::FLOAT, "a_Fade"}, 
            });
    s_RenderData.CircleVertexBuffer->SetBufferLayout(CircleLayout);
    s_RenderData.CircleVertexArray->AddVertexBuffer(s_RenderData.CircleVertexBuffer);
    s_RenderData.CircleVertexBufferBase = new CircleVertex[s_RenderData.MaxVertices];
    s_RenderData.CircleVertexArray->SetIndexBuffer(s_RenderData.QuadIndexBuffer);

    s_RenderData.WhiteTexture = Texture::Create(1,1);
    uint8_t whitePixel[4] = { 255, 255, 255, 255 }; 
    s_RenderData.WhiteTexture->SetData(whitePixel, sizeof(whitePixel));

    s_RenderData.LineVertexArray = VertexArray::Create();
    s_RenderData.LineVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(LineVertex));
    BufferLayout LineLayout = BufferLayout({
            { SHADER_DATA_TYPE::FLOAT3, "a_Position"}, 
            { SHADER_DATA_TYPE::FLOAT4, "a_Color"}, 
            });
    s_RenderData.LineVertexBuffer->SetBufferLayout(LineLayout);
    s_RenderData.LineVertexArray->AddVertexBuffer(s_RenderData.LineVertexBuffer);
    s_RenderData.LineVertexBufferBase = new LineVertex[s_RenderData.MaxVertices];

    s_RenderData.QuadShader = m_ShaderManager->LoadShader("QuadShader", "assets/shaders/QuadShader.glsl");
    s_RenderData.LineShader = m_ShaderManager->LoadShader("LineShader", "assets/shaders/LineShader.glsl");
    s_RenderData.CircleShader = m_ShaderManager->LoadShader("CircleShader", "assets/shaders/CircleShader.glsl");
    if(!s_RenderData.QuadShader) {
        BIT_LOG_ERROR("Failed to load QuadShader!");
    }
    if(!s_RenderData.LineShader) {
        BIT_LOG_ERROR("Failed to load LineShader!");
    }
    if(!s_RenderData.CircleShader) {
        BIT_LOG_ERROR("Failed to load CircleShader!");
    }
    i32 samplers[s_RenderData.MaxTextureSlots];
    for(u32 i = 0; i < s_RenderData.MaxTextureSlots; ++i)
    {
        samplers[i] = i;
    }
    s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;
    s_RenderData.QuadShader->Bind();
    s_RenderData.QuadShader->SetIntArray("u_Textures", samplers, s_RenderData.MaxTextureSlots);

    s_RenderData.QuadVertexPositions[0]= {-0.5f, -0.5f, 0.0f, 1.0f};
    s_RenderData.QuadVertexPositions[1]= { 0.5f, -0.5f, 0.0f, 1.0f};
    s_RenderData.QuadVertexPositions[2]= { 0.5f,  0.5f, 0.0f, 1.0f};
    s_RenderData.QuadVertexPositions[3]= {-0.5f,  0.5f, 0.0f, 1.0f};

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

void Renderer2D::BeginScene(const BMath::Mat4& viewProjectionMatrix, const BMath::Vec4& color)
{

    m_CurrentViewProjectionMatrix = viewProjectionMatrix;

    s_RenderData.QuadShader->Bind();
    s_RenderData.QuadShader->SetMat4("u_ViewProjection", m_CurrentViewProjectionMatrix);

    s_RenderData.LineShader->Bind();
    s_RenderData.LineShader->SetMat4("u_ViewProjection", m_CurrentViewProjectionMatrix);

    s_RenderData.CircleShader->Bind();
    s_RenderData.CircleShader->SetMat4("u_ViewProjection", m_CurrentViewProjectionMatrix);

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

    s_RenderData.CircleIndexCount = 0;
    s_RenderData.CircleVertexBufferPtr = s_RenderData.CircleVertexBufferBase;

    s_RenderData.LineVertexCount= 0;
    s_RenderData.LineVertexBufferPtr = s_RenderData.LineVertexBufferBase;

    Stats.QuadCount = 0;
    Stats.DrawCalls = 0;
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
        Stats.DrawCalls++;
    }
    if(s_RenderData.CircleIndexCount)
    {
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.CircleVertexBufferPtr - (uint8_t*)s_RenderData.CircleVertexBufferBase);
        s_RenderData.CircleVertexBuffer->SetData(s_RenderData.CircleVertexBufferBase, dataSize);

        s_RenderData.CircleShader->Bind();
        m_RenderCommand->DrawIndexed(s_RenderData.CircleVertexArray, s_RenderData.CircleIndexCount);
        Stats.DrawCalls++;
    }
    if(s_RenderData.LineVertexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.LineVertexBufferPtr - (uint8_t*)s_RenderData.LineVertexBufferBase);
        s_RenderData.LineVertexBuffer->SetData(s_RenderData.LineVertexBufferBase, dataSize);

        s_RenderData.LineShader->Bind();
        m_RenderCommand->SetLineWidth(s_RenderData.LineWidth);
        m_RenderCommand->DrawLine(s_RenderData.LineVertexArray, s_RenderData.LineVertexCount);

        Stats.DrawCalls++;
    }
}

void Renderer2D::DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, f32 rotation, const BMath::Vec4& color)
{
    BMath::Mat4 transform = BMath::Mat4CreateTransform(position, size, {0.0f, 0.0f, rotation});
    DrawQuad(transform, color);
}
void Renderer2D::DrawQuad(BMath::Mat4& transform, const BMath::Vec4& color)
{
    f32 textureIndex = 0.0f;
    BMath::Vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    for(u32 i = 0; i < 4; ++i)
    {
        s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->TexCoords = texCoords[i];
        s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RenderData.QuadVertexBufferPtr++; 
    }

    s_RenderData.QuadIndexCount += 6;

    Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, f32 rotation, Texture* sprite, f32* uvs)
{
    BMath::Mat4 transform = BMath::Mat4CreateTransform(position, size, {0.0f, 0.0f, rotation});
    DrawQuad(transform, sprite, uvs);
}
void Renderer2D::DrawQuad(BMath::Mat4& transform, Texture* sprite, f32* uvs)
{
    float textureIndex = 0.0f;
    const BMath::Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    BMath::Vec2 texCoords[4] = { {uvs[0], uvs[1]}, {uvs[2], uvs[3]},{uvs[4], uvs[5]},{uvs[6], uvs[7]}};
    //Check if we already have the texture stored inside the texture slots to be bound in future
    for(u32 i = 0; i < s_RenderData.TextureSlotIndex; ++i)
    {
        if(s_RenderData.TextureSlots[i]->GetID() == sprite->GetID())
        {
            textureIndex = (f32)i;
            break;
        }
    }
    // if no texture found inside the texture slots we already have add it to be bound and used in future
    if(textureIndex == 0.0f)
    {
        textureIndex = s_RenderData.TextureSlotIndex;
        s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = sprite;
        s_RenderData.TextureSlotIndex++;
    }

    for(u32 i = 0; i < 4; ++i)
    {
        s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->TexCoords = texCoords[i];
        s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RenderData.QuadVertexBufferPtr++; 
    }
    s_RenderData.QuadIndexCount += 6;
    Stats.QuadCount++;
}
void Renderer2D::DrawCircle(BMath::Mat4& transform, const BMath::Vec4& color, f32 thickness, f32 fade) 
{
    for(u32 i = 0; i < 4; ++i)
    {
        s_RenderData.CircleVertexBufferPtr->WorldPosition = transform * s_RenderData.QuadVertexPositions[i];
        s_RenderData.CircleVertexBufferPtr->NormalizedPosition = BMath::Vec4ToVec3(s_RenderData.QuadVertexPositions[i]) * 2.0f;
        s_RenderData.CircleVertexBufferPtr->Color = color;
        s_RenderData.CircleVertexBufferPtr->Thickness = thickness;
        s_RenderData.CircleVertexBufferPtr->Fade = fade;
        s_RenderData.CircleVertexBufferPtr++; 
        
       
    }

    s_RenderData.CircleIndexCount += 6;

    Stats.QuadCount++;
}
void Renderer2D::DrawLine(const BMath::Vec3& p0, const BMath::Vec3& p1, const BMath::Vec4& color)
{
    s_RenderData.LineVertexBufferPtr->Position = p0;
    s_RenderData.LineVertexBufferPtr->Color= color;
    s_RenderData.LineVertexBufferPtr++;

    s_RenderData.LineVertexBufferPtr->Position = p1;
    s_RenderData.LineVertexBufferPtr->Color= color;
    s_RenderData.LineVertexBufferPtr++;

    s_RenderData.LineVertexCount += 2;
}
void Renderer2D::DrawRect(const BMath::Vec3& position, const BMath::Vec3& size, f32 rotation, const BMath::Vec4& color)
{
    BMath::Mat4 transform = BMath::Mat4CreateTransform(position, size, rotation);
    DrawRect(transform, color);
}
void Renderer2D::DrawRect(const BMath::Mat4& transform, const BMath::Vec4& color)
{
    BMath::Vec3 worldVertices[4];
    for (u32 i = 0; i < 4; ++i)
    {
        BMath::Vec4 transformed = transform * s_RenderData.QuadVertexPositions[i];
        worldVertices[i] = BMath::Vec4ToVec3(transformed);
    }

    DrawLine(worldVertices[0], worldVertices[1], color);
    DrawLine(worldVertices[1], worldVertices[2], color);
    DrawLine(worldVertices[2], worldVertices[3], color);
    DrawLine(worldVertices[3], worldVertices[0], color);
}

void Renderer2D::DrawCapsule(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius, const BMath::Vec4& color)
{
    BMath::Vec2 dir = center2 - center1;
    f32 length = BMath::Vec2Length(dir);
    if(BMath::Vec2Length(dir) <= 0.01f) // circles too close draw only one circle
    {
        BMath::Mat4 transform = BMath::Mat4Translate(center1.x, center1.y, 0.0f)
                             * BMath::Mat4Scale(radius * 2.0f, radius * 2.0f, 1.0f);
                             
        DrawCircle(transform, color);
        return;
    }
    BMath::Vec2 rectMid = (center1 + center2) * 0.5f;
    f32 angle = atan2f(dir.y, dir.x);

    BMath::Mat4 rectTransform = BMath::Mat4Translate(rectMid.x, rectMid.y, 0.0f) * BMath::Mat4Rotate(0.0f, 0.0f, angle) * BMath::Mat4Scale(length, radius * 2.0f, 0.0f);
    DrawRect(rectTransform,  color);

    BMath::Mat4 leftCapTransform  = BMath::Mat4Translate(center1.x, center1.y, 0.0f) * BMath::Mat4Scale(radius * 2.0f, radius * 2.0f, 1.0f);
    
    BMath::Mat4 rightCapTransform = BMath::Mat4Translate(center2.x, center2.y, 0.0f) * BMath::Mat4Scale(radius * 2.0f, radius * 2.0f, 1.0f);
    
    DrawCircle(leftCapTransform, color, 0.2f);

    DrawCircle(rightCapTransform, color, 0.2f);
}
void Renderer2D::Shutdown()
{
    delete m_ShaderManager;

    delete s_RenderData.QuadVertexArray;
    delete s_RenderData.WhiteTexture;
    delete[] s_RenderData.QuadVertexBufferBase;

    delete s_RenderData.CircleVertexArray;
    delete[] s_RenderData.CircleVertexBufferBase;

    delete s_RenderData.LineVertexArray;
    delete[] s_RenderData.LineVertexBufferBase;


    delete s_RenderData.QuadIndexBuffer;
    delete m_RenderCommand;
}
} 
