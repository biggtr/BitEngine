#pragma once
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Compontents.h"

namespace BitEngine
{
class Shader;
class VertexBuffer;
class IndexBuffer;
struct CCamera;
class Texture;

typedef enum
{
    RENDER_FILL,
    RENDER_LINE,
} RENDER_MODE;
class Renderer2D
{
private:
    RenderCommand* m_RenderCommand;
    CCamera* m_Camera2D;
    BMath::Mat4 m_ProjectionMatrix;
    f32 m_ZNear, m_ZFar;
    
public:
    Renderer2D(){}
    ~Renderer2D(){}

    b8 Initialize();
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::Vec4& color) const;
    void Clear() const;

    void BeginScene(const BMath::Mat4& viewMatrix);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, CSprite& sprite);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, const BMath::Vec4& color);
    void DrawQuad(BMath::Mat4& transform, const BMath::Vec4& color);
    void DrawQuad(BMath::Mat4& transform, CSprite& sprite);

    void DrawLine(const BMath::Vec3& p0, const BMath::Vec3& p1, const BMath::Vec4& color);
    void DrawRect(const BMath::Vec3& position, const BMath::Vec3& size, const BMath::Vec4& color);
    // void DrawRect(BMath::Mat4& transform, const BMath::Vec4& color)
    void DrawCircle(BMath::Mat4& transform, const BMath::Vec4& color, f32 thickness = 0.5, f32 fade = 0.0005);
    void EndScene();

    

    void Shutdown();

    void OnWindowResize(u16 width, u16 height);
private:
    void StartBatch();
    void Flush();
};
}
