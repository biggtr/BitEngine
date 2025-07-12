#include "OpenGLRendererAPI.h"
#include "Bit/Core/Core.h"
#include "Bit/Renderer/VertexArray.h"

namespace BitEngine
{


void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
     GLCall(glViewport(x, y, width, height));
}
void OpenGLRendererAPI::SetClearColor(const BitMath::Vector4& color) const 
{
    GLCall(glClearColor(color.x, color.y, color.z, color.w));
}
void OpenGLRendererAPI::Clear() const 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
void OpenGLRendererAPI::DrawIndexed(const VertexArray* VAO)  const
{
    VAO->Bind();
    auto IBO = VAO->GetIndexBuffer();
    IBO->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
}
}
