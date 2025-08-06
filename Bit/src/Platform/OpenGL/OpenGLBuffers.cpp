#include "OpenGLBuffers.h"
#include <cstdint>
#include <glad/glad.h>
#include "Bit/Core/Core.h"
namespace BitEngine
{

///////////////////////////////////////////////////////////////////////
/// VertexBuffer
///////////////////////////////////////////////////////////////////////
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}
OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}
void OpenGLVertexBuffer::Bind()

{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}
        
void OpenGLVertexBuffer::UnBind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::SetData(void* data, uint32_t size) 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

///////////////////////////////////////////////////////////////////////
/// IndexBuffer 
///////////////////////////////////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
{
    m_Count = count;
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices ,  GL_STATIC_DRAW));
}
void OpenGLIndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}
        
void OpenGLIndexBuffer::UnBind() const 
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
}
