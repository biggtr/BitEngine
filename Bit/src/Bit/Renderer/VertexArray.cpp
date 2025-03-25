#include "VertexArray.h"
#include "glad/glad.h"

namespace BitEngine 
{
VertexArray::VertexArray()
{
   glGenVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_ID);
}
void VertexArray::UnBind()
{
    glBindVertexArray(0);
}
void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    glBindVertexArray(m_ID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}
void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    glBindVertexArray(m_ID);
    vertexBuffer->Bind();
    const BufferLayout& layout = vertexBuffer->GetBufferLayout();
    auto elements = layout.GetBufferElements();
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
                    glEnableVertexAttribArray(location);  
                    glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_FLOAT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout.GetStride(), 
                            (const void*) element.Offset);
                    break;
                }
            case SHADER_DATA_TYPE::INT2:
            case SHADER_DATA_TYPE::INT3: 
            case SHADER_DATA_TYPE::INT4:  
                {
                    glEnableVertexAttribArray(location);  
                    glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_INT, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout.GetStride(), 
                            (const void*) element.Offset);
                    break;
                }
           
            case SHADER_DATA_TYPE::BOOL:   
                {
                    glEnableVertexAttribArray(location);  
                    glVertexAttribPointer(location, 
                            element.GetComponentCount(), 
                            GL_BOOL, 
                            element.Normalized ? GL_TRUE : GL_FALSE, 
                            layout.GetStride(), 
                            (const void*) element.Offset);
                    break;
                }
            case SHADER_DATA_TYPE::NONE:    
                break;
                     

        }

    }

    m_VertexBuffers.push_back(vertexBuffer);

}
}
