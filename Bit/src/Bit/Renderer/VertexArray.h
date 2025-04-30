#pragma once


namespace BitEngine
{

class VertexBuffer;
class IndexBuffer;

class VertexArray
{

private:
    unsigned int m_ID;
    unsigned int m_VertexBufferIndex;
    std::vector<VertexBuffer*> m_VertexBuffers;
    IndexBuffer* m_IndexBuffer;
    


public:
    VertexArray();
    ~VertexArray();
    unsigned int GetID() { return m_ID; }
    void Bind();
    void UnBind();

    void AddVertexBuffer(VertexBuffer* vertexBuffer);
    void SetIndexBuffer(IndexBuffer* indexBuffer); 

    const std::vector<VertexBuffer*> GetVertexBuffer() const { return m_VertexBuffers; }
    const IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; };
    

private:

};

}
