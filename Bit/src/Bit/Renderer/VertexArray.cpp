#include "VertexArray.h"
#include "Bit/Renderer/Buffers.h"
#include "glad/glad.h"
#include "Bit/Core/Core.h"

namespace BitEngine 
{
VertexArray::VertexArray()
{
   GLCall(glGenVertexArrays(1, &m_ID));
}

VertexArray::~VertexArray()
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

void VertexArray::Bind()
{
    GLCall(glBindVertexArray(m_ID));
}
void VertexArray::UnBind()
{
    GLCall(glBindVertexArray(0));
}
void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    GLCall(glBindVertexArray(m_ID));
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}
void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLCall(glBindVertexArray(m_ID));
    vertexBuffer->Bind();
    auto layout = vertexBuffer->GetBufferLayout();
    auto elements = layout->GetBufferElements();
    for(int location{}; location < elements.size(); location++)
    {
        auto element = elements[location];
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
                    GLCall(glEnableVertexAttribArray(location));  
                    GLCall(glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_FLOAT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    break;
                }
            case SHADER_DATA_TYPE::INT2:
            case SHADER_DATA_TYPE::INT3: 
            case SHADER_DATA_TYPE::INT4:  
                {
                    GLCall(glEnableVertexAttribArray(location));  
                    GLCall(glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_FLOAT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    break;
                }
           
            case SHADER_DATA_TYPE::BOOL:   
                {
                    GLCall(glEnableVertexAttribArray(location));  
                    GLCall(glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_FLOAT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout->GetStride(), 
                            (const void*) element.Offset));
                    break;
                }
            case SHADER_DATA_TYPE::NONE:    
                break;
                     

        }

    }

    m_VertexBuffers.push_back(vertexBuffer);

}

}
