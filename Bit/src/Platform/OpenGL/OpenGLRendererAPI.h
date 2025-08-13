#pragma once
#include "Bit/Renderer/RendererAPI.h"
#include <cstdint>

namespace BitEngine
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    OpenGLRendererAPI() {}

    virtual void Init() override{}
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;
    virtual void SetClearColor(const BMath::Vec4& color) const override;
    virtual void Clear() const override;

    virtual void DrawLine(const VertexArray* VAO, u32 vertexCount) override;
    virtual void SetLineWidth(f32 width) override;

    virtual void DrawIndexed(const VertexArray* VAO, uint32_t indexCount = 0) const override;
};
}
