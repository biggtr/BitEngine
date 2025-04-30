#include "Buffers.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
namespace BitEngine 
{
/////////////////////////////////////////////////////////////
// VertexBuffer
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(float* vertices, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(m_ID, size, vertices, GL_STATIC_DRAW));
}
void VertexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}
        
void VertexBuffer::UnBind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    delete m_BufferLayout;
}

/////////////////////////////////////////////////////////////
// IndexBuffer 
////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count)
{
    m_Count = count;
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices ,  GL_STATIC_DRAW));
}
void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}
        
void IndexBuffer::UnBind() const 
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
}
