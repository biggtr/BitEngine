#include "glad/glad.h"
#include <vector>


namespace BitEngine
{

struct BufferElement
{

};

class BufferLayout
{
private:
    std::vector<BufferElement> m_BufferElements;
};

class VertexBuffer
{
private:
    unsigned int m_ID;
    BufferLayout m_BufferLayout;

public:
    VertexBuffer(float* vertices, unsigned int size);
    void Bind();
    void UnBind();
    void SetBufferLayout(BufferLayout& bufferLayout) { m_BufferLayout = bufferLayout;}
    const BufferLayout& GetBufferLayout() { return m_BufferLayout;}

private: 

};

class IndexBuffer
{
private:
    unsigned int m_ID;
    unsigned int m_Count;

public:
    
    IndexBuffer(float* vertices, unsigned int count);
    void Bind();
    void UnBind();
    unsigned int GetCount(){ return m_Count;}

private: 

};
}
