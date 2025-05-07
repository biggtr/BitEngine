#pragma once
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/RendererAPI.h"

namespace BitEngine
{
class Shader;
class VertexBuffer;
class IndexBuffer;

class Renderer2D
{
public:
    Renderer2D(RendererAPI* api);
    ~Renderer2D();

    void Init();
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BitMath::Vector4& color) const;
    void Clear() const;

    void BeginScene();
    void DrawQuad(BitMath::Vector4 color);
    void EndScene();

private:
    RenderCommand* m_RenderCommand;
    VertexArray* m_QuadVAO;
    VertexBuffer* m_QuadVBO;
    IndexBuffer* m_QuadIBO;
    Shader* m_QuadShader;
    
};
}
