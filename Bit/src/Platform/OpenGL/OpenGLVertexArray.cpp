#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
namespace BitEngine 
{
OpenGLVertexArray::OpenGLVertexArray()
    : m_VertexBufferIndex(0), m_IndexBuffer(nullptr)
{
    // GLCall(glCreateVertexArrays(1, &m_ID));
    GLCall(glGenVertexArrays(1, &m_ID));
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    for(auto buffer : m_VertexBuffers)
    {
        delete buffer;
    }
    m_VertexBuffers.clear();

    if(m_IndexBuffer)
    {
        delete m_IndexBuffer;
    }
}

void OpenGLVertexArray::Bind()
{
    GLCall(glBindVertexArray(m_ID));
}
void OpenGLVertexArray::UnBind()
{
    GLCall(glBindVertexArray(0));
}
void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    GLCall(glBindVertexArray(m_ID));
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}
void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLCall(glBindVertexArray(m_ID));
    vertexBuffer->Bind();
    const auto& layout = vertexBuffer->GetBufferLayout();
    auto elements = layout->GetBufferElements();
    for(const auto& element : elements)
    {
        switch (element.Type) 
        {
            case SHADER_DATA_TYPE::FLOAT:   
            case SHADER_DATA_TYPE::FLOAT2: 
            case SHADER_DATA_TYPE::FLOAT3:   
            case SHADER_DATA_TYPE::FLOAT4:   
            case SHADER_DATA_TYPE::MAT2:     
            case SHADER_DATA_TYPE::MAT3:     
            case SHADER_DATA_TYPE::MAT4:     
                {
                    std::cout << m_VertexBufferIndex << "Vertex Index\n";
                    GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));  
                    GLCall(glVertexAttribPointer(m_VertexBufferIndex, 
                            element.GetComponentCount(), 
                            GL_FLOAT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    m_VertexBufferIndex++;
                    break;
                }
            case SHADER_DATA_TYPE::INT2:
            case SHADER_DATA_TYPE::INT3: 
            case SHADER_DATA_TYPE::INT4:  
                {
                    GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));  
                    GLCall(glVertexAttribPointer(m_VertexBufferIndex, 
                            element.GetComponentCount(), 
                            GL_INT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    m_VertexBufferIndex++;
                    break;
                }
           
            case SHADER_DATA_TYPE::BOOL:   
                {
                    GLCall(glEnableVertexAttribArray(m_VertexBufferIndex));  
                    GLCall(glVertexAttribPointer(m_VertexBufferIndex, 
                            element.GetComponentCount(), 
                            GL_BOOL, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    m_VertexBufferIndex++;
                    break;
                }
            case SHADER_DATA_TYPE::NONE:    
                break;
                     

        }

    }

    m_VertexBuffers.push_back(vertexBuffer);

}

}
