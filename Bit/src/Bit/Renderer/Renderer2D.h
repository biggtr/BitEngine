#pragma once
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Compontents.h"

namespace BitEngine
{
class Shader;
class VertexBuffer;
class IndexBuffer;
struct Camera2DComponent;
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
    Camera2DComponent* m_Camera2D;
    BMath::Mat4 m_CurrentViewProjectionMatrix;
    
public:
    Renderer2D(){}
    ~Renderer2D(){}

    b8 Initialize();
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::Vec4& color) const;
    void Clear() const;

    void BeginScene(const BMath::Mat4& viewProjectionMatrix);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, SpriteComponent& sprite);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& size, const BMath::Vec4& color);
    void DrawQuad(BMath::Mat4& transform, const BMath::Vec4& color);
    void DrawQuad(BMath::Mat4& transform, SpriteComponent& sprite);

    void DrawLine(const BMath::Vec3& p0, const BMath::Vec3& p1, const BMath::Vec4& color);
    void DrawRect(const BMath::Vec3& position, const BMath::Vec3& size, const BMath::Vec4& color);
    // void DrawRect(BMath::Mat4& transform, const BMath::Vec4& color)
    void DrawCircle(BMath::Mat4& transform, const BMath::Vec4& color, f32 thickness = 0.5, f32 fade = 0.0005);
    void EndScene();

    

    void Shutdown();

private:
    void StartBatch();
    void Flush();
};
}
