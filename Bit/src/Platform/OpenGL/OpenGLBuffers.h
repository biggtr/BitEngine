#pragma once
#include "Bit/Renderer/Buffers.h"
namespace BitEngine
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(float* vertices, unsigned int size);

    virtual void Bind() override;
    virtual void UnBind() override;
    virtual void SetBufferLayout(const BufferLayout& bufferLayout) override { m_Layout = bufferLayout; }
    virtual const BufferLayout& GetBufferLayout() const override { return m_Layout; };

private:
    unsigned int m_ID;
    BufferLayout m_Layout;


};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(unsigned int* indices, unsigned int count);

    virtual ~OpenGLIndexBuffer() = default;
    virtual void Bind() const override;
    virtual void UnBind() const override;
    virtual unsigned int GetCount() const override { return m_Count; };
private:
    unsigned int m_ID;
    unsigned int m_Count;

};
}
