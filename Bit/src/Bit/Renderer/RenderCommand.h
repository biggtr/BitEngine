#pragma once
#include "Bit/Renderer/RendererAPI.h"
namespace BitEngine
{
class RenderCommand 
{
public:
    RenderCommand(){}
    ~RenderCommand();

    void Init(RendererAPI* api);

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BitMath::Vector4& color) const;
    void Clear() const; 
    void DrawIndexed(const VertexArray* VAO) const;

private:
    RendererAPI* m_API;
    
};
}
