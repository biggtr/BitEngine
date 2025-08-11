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
class Renderer2D
{
private:
    RenderCommand* m_RenderCommand;
    CCamera* m_Camera2D;
public:
    Renderer2D(){}
    ~Renderer2D(){}

    b8 Initialize();
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::Vec4& color) const;
    void Clear() const;

    void BeginScene(CCamera* camera2D);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& scale, const BMath::Vec4& color);
    void DrawQuad(const BMath::Vec3& position, const BMath::Vec3& scale, CSprite& sprite);
    void EndScene();

    void Shutdown();

private:
    void StartBatch();
    void Flush();
    
};
}
