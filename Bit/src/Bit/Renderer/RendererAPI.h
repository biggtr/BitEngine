#pragma once

#include <cstdint>
namespace BitEngine
{

enum class RENDERER_API
{
    NONE, OPENGL
};
class RendererAPI
{

public:
    virtual ~RendererAPI() = default;
    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor() = 0;

    static RENDERER_API GetAPI() { return s_API; }
    static void SetAPI(RENDERER_API rendererAPI) { s_API = rendererAPI; }

    static RendererAPI* Create();
    

private:
    static RENDERER_API s_API;

};
}
