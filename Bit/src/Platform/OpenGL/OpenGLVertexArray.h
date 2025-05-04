#pragma once
#include "Bit/Renderer/VertexArray.h"
namespace BitEngine 
{

class OpenGLVertexArray : public VertexArray
{

    


public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray(); 
    virtual void Bind() const override;
    virtual void UnBind() const override;
    virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
    virtual void SetIndexBuffer(IndexBuffer* indexBuffer) override;
    virtual const std::vector<VertexBuffer*> GetVertexBuffer() const override { return m_VertexBuffers; };
    virtual const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer; }    

private:
    unsigned int m_ID;
    unsigned int m_VertexBufferIndex;
    std::vector<VertexBuffer*> m_VertexBuffers;
    IndexBuffer* m_IndexBuffer;

};
}
