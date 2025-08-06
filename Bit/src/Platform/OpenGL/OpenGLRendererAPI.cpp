#include "OpenGLRendererAPI.h"
#include "Bit/Core/Core.h"
#include "Bit/Renderer/VertexArray.h"
#include <cstdint>

namespace BitEngine
{


void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
     GLCall(glViewport(x, y, width, height));
}
void OpenGLRendererAPI::SetClearColor(const BMath::vec4& color) const 
{
    GLCall(glClearColor(color.x, color.y, color.z, color.w));
}
void OpenGLRendererAPI::Clear() const 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void OpenGLRendererAPI::DrawIndexed(const VertexArray* VAO, uint32_t indexCount)  const
{
    uint32_t count = indexCount ? indexCount : 0;
    VAO->Bind();
    auto IBO = VAO->GetIndexBuffer();
    IBO->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}
}
