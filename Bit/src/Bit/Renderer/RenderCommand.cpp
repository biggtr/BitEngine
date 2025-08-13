#include "RenderCommand.h"
#include "Bit/Renderer/Renderer2D.h"
#include <cstdint>

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
void RenderCommand::SetClearColor(const BMath::Vec4& color) const 
{
    m_API->SetClearColor(color);
}
void RenderCommand::Clear() const 
{
    m_API->Clear();
}
void RenderCommand::DrawIndexed(const VertexArray* VAO, uint32_t indexCount) const
{
    m_API->DrawIndexed(VAO, indexCount);
}
void RenderCommand::DrawLine(const VertexArray* VAO, u32 vertexCount)
{
    m_API->DrawLine(VAO, vertexCount);

}
void RenderCommand::SetLineWidth(f32 width) 
{
    m_API->SetLineWidth(width);
}
RenderCommand::~RenderCommand()
{
    delete m_API;
}
}
