#include "Buffers.h"
#include <cstdint>
#include <glad/glad.h>
#include "Bit/Core/Core.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Backend/OpenGL/OpenGLBuffers.h"

namespace BitEngine
{

VertexBuffer* VertexBuffer::Create(uint32_t size)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLVertexBuffer(size);
      break;
    }
    return nullptr;
}
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

IndexBuffer* IndexBuffer::Create(u32 count)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLIndexBuffer(count);
      break;
    }
    return nullptr;
}
IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
