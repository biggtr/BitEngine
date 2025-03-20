#include <vector>
#include "Buffers.h"
namespace BitEngine
{


class VertexArray
{

private:
    unsigned int m_ID;
    std::vector<VertexBuffer*> m_VertexBuffers;
    IndexBuffer* m_IndexBuffer;
    


public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void UnBind();

    void AddVertexBuffer(VertexBuffer* vertexBuffer);
    void SetIndexBuffer(IndexBuffer* indexBuffer); 

private:

};

}
