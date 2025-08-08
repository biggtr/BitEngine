#pragma once
#include "Bit/Renderer/RendererAPI.h"
#include <cstdint>
namespace BitEngine
{
class RenderCommand 
{
public:
    RenderCommand(){}
    ~RenderCommand();

    void Init(RendererAPI* api);

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::Vec4& color) const;
    void Clear() const; 
    void DrawIndexed(const VertexArray* VAO, uint32_t indexCount = 0) const;

private:
    RendererAPI* m_API;
    
};
}
