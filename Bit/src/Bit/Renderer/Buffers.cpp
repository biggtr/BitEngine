#include "Buffers.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffers.h"

namespace BitEngine
{

VertexBuffer* VertexBuffer::Create(float *vertices, unsigned int size)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLVertexBuffer(vertices, size);
      break;
    }
    return nullptr;
}
IndexBuffer* IndexBuffer::Create(unsigned int * indices, unsigned int count)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLIndexBuffer(indices, count);
      break;
    }
    return nullptr;
}
}
