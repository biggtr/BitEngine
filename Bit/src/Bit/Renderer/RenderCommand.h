#pragma once
#include "Bit/Math/Vector.h"
#include <cstdint>
namespace BitEngine
{
class RendererAPI;
class VertexArray;
class RenderCommand 
{
public:
    RenderCommand(){}
    ~RenderCommand();

    b8 Initialize();

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void SetClearColor(const BMath::Vec4& color) const;
    void Clear() const; 
    void DrawIndexed(const VertexArray* VAO, uint32_t indexCount = 0) const;
    void DrawLine(const VertexArray* VAO, u32 vertexCount);
    void SetLineWidth(f32 width) ;

private:
    RendererAPI* m_API;
    
};
}
