#include "RenderCommand.h"

namespace BitEngine 
{


void RenderCommand::Init(RendererAPI* api)
{
    m_API = api;
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
    m_API->SetViewport(x, y, width, height);
}
void RenderCommand::SetClearColor(const BMath::Vector4& color) const 
{
    m_API->SetClearColor(color);
}
void RenderCommand::Clear() const 
{
    m_API->Clear();
}
void RenderCommand::DrawIndexed(const VertexArray* VAO) const
{
    m_API->DrawIndexed(VAO);
}
RenderCommand::~RenderCommand()
{
    delete m_API;
}
}
