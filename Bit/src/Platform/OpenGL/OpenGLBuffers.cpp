#include "OpenGLBuffers.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
namespace BitEngine
{

///////////////////////////////////////////////////////////////////////
/// VertexBuffer
///////////////////////////////////////////////////////////////////////
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

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    delete m_Layout;
}

///////////////////////////////////////////////////////////////////////
/// IndexBuffer 
///////////////////////////////////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count)
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
