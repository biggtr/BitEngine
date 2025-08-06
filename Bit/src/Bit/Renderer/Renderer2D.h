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
    Renderer2D();
    ~Renderer2D();

    void Init();
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::vec4& color) const;
    void Clear() const;

    void BeginScene(CCamera* camera2D);
    void DrawQuad(const BMath::vec3& position, const BMath::vec3& scale, const BMath::vec4& color);
    void DrawQuad(const BMath::vec3& position, const BMath::vec3& scale, const BMath::vec4& color, Texture* texture);
    void EndScene();

private:
    void StartBatch();
    void Flush();
    
};
}
