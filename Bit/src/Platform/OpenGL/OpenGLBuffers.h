#pragma once
#include "Bit/Renderer/Buffers.h"
#include <cstdint>
namespace BitEngine
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, unsigned int size);


    virtual void Bind() override;
    virtual void UnBind() override;
    virtual void SetData(void* data, uint32_t size) override;
    virtual void SetBufferLayout(const BufferLayout& bufferLayout) override { m_Layout = bufferLayout; }
    virtual const BufferLayout& GetBufferLayout() const override { return m_Layout; };
    virtual u32 GetVertexCount() const override { return m_VertexCount; }
private:
    unsigned int m_ID;
    BufferLayout m_Layout;
    u32 m_VertexCount;

};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

    virtual ~OpenGLIndexBuffer() = default;
    virtual void Bind() const override;
    virtual void UnBind() const override;
    virtual unsigned int GetCount() const override { return m_Count; };
private:
    unsigned int m_ID;
    unsigned int m_Count;

};
}
