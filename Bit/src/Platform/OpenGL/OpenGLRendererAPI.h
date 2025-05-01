#pragma once
#include "Bit/Renderer/RendererAPI.h"

namespace BitEngine
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    OpenGLRendererAPI();

    virtual void Init() override{}
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override{}
    virtual void SetClearColor() override{}
};
}
