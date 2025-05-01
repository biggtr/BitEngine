#pragma once
#include "Buffers.h"

namespace BitEngine
{


class VertexArray
{

private:
    unsigned int m_ID;
    unsigned int m_VertexBufferIndex;
    std::vector<VertexBuffer*> m_VertexBuffers;
    IndexBuffer* m_IndexBuffer;
    


public:
    virtual ~VertexArray() = default;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
    virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0; 

    virtual const std::vector<VertexBuffer*> GetVertexBuffer() const = 0; 
    virtual const IndexBuffer* GetIndexBuffer() const = 0; 
    
    static VertexArray* Create();

private:

};

}
