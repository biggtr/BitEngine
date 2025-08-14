#include "OpenGLRendererAPI.h"
#include "Bit/Core/Core.h"
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/VertexArray.h"
#include <GL/gl.h>
#include <cstdint>

namespace BitEngine
{

b8 OpenGLRendererAPI::Init()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}
void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const 
{
     GLCall(glViewport(x, y, width, height));
}
void OpenGLRendererAPI::SetClearColor(const BMath::Vec4& color) const 
{
    GLCall(glClearColor(color.x, color.y, color.z, color.w));
}
void OpenGLRendererAPI::Clear() const 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


void OpenGLRendererAPI::DrawLine(const VertexArray* VAO, u32 vertexCount)
{
    VAO->Bind();
    GLCall(glDrawArrays(GL_LINES, 0,vertexCount));
}
void OpenGLRendererAPI::SetLineWidth(f32 width)
{
    GLCall(glLineWidth(width));
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
