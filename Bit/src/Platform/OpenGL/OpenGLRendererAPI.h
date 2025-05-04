#pragma once
#include "Bit/Renderer/RendererAPI.h"

namespace BitEngine
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    OpenGLRendererAPI() {}

    virtual void Init() override{}
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;
    virtual void SetClearColor(const BitMath::Vector4& color) const override;
    virtual void Clear() const override;

    virtual void DrawIndexed(const VertexArray* VAO) const override;
};
}
