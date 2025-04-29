#include "Buffers.h"
#include <glad/glad.h>

namespace BitEngine 
{
/////////////////////////////////////////////////////////////
// VertexBuffer
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(float* vertices, unsigned int size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(m_ID, size, vertices, GL_STATIC_DRAW);
}
void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
        
void VertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices ,  GL_STATIC_DRAW);
}
void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}
        
void IndexBuffer::UnBind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}
