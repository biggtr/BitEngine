#pragma once

#include <cstdint>
#include "Bit/Math/Vector.h"
namespace BitEngine
{
class VertexArray;
enum class RENDERER_API
{
    NONE, OPENGL
};
class RendererAPI
{

public:
    virtual ~RendererAPI() = default;
    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;
    virtual void SetClearColor(const BMath::Vector4& color) const = 0;
    virtual void Clear() const = 0;

    virtual void DrawIndexed(const VertexArray* VAO) const = 0;

    static RENDERER_API GetAPI() { return s_API; }
    static void SetAPI(RENDERER_API rendererAPI) { s_API = rendererAPI; }

    static RendererAPI* Create();
    

private:
    static RENDERER_API s_API;

};
}
