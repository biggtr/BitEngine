#pragma once

#include "Bit/Core/Defines.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace BitEngine

{
    enum class SHADER_DATA_TYPE
    {
        NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT2, MAT3, MAT4, INT2, INT3, INT4, BOOL,
    };

    static uint32_t GetShaderDataTypeSize(SHADER_DATA_TYPE type) 
    {
        switch (type)
        {
            case SHADER_DATA_TYPE::FLOAT:   return 4 * 1;
            case SHADER_DATA_TYPE::FLOAT2:  return 4 * 2;
            case SHADER_DATA_TYPE::FLOAT3:  return 4 * 3;
            case SHADER_DATA_TYPE::FLOAT4:  return 4 * 4;
            case SHADER_DATA_TYPE::MAT2:    return 4 * 4;
            case SHADER_DATA_TYPE::MAT3:    return 4 * 3 * 3;
            case SHADER_DATA_TYPE::MAT4:    return 4 * 4 * 4;
            case SHADER_DATA_TYPE::INT2:    return 4 * 2;
            case SHADER_DATA_TYPE::INT3:    return 4 * 3;
            case SHADER_DATA_TYPE::INT4:    return 4 * 4;
            case SHADER_DATA_TYPE::BOOL:    return 1;
            case SHADER_DATA_TYPE::NONE:    return 0;
        }
        return 0;
    }

struct BufferElement
{
    std::string AttributeName;
    SHADER_DATA_TYPE Type;
    uint32_t Size;
    size_t Offset;
    bool Normalized;
    

    BufferElement(SHADER_DATA_TYPE type, const std::string& attributeName,bool normalized = false)
        : AttributeName(attributeName), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
    {

    }
    uint32_t GetComponentCount() const 
    {
        switch (Type)
        {
            case SHADER_DATA_TYPE::FLOAT:   return 1;
            case SHADER_DATA_TYPE::FLOAT2:  return 2;
            case SHADER_DATA_TYPE::FLOAT3:  return 3;
            case SHADER_DATA_TYPE::FLOAT4:  return 4;
            case SHADER_DATA_TYPE::MAT2:    return 4;
            case SHADER_DATA_TYPE::MAT3:    return 3;
            case SHADER_DATA_TYPE::MAT4:    return 4;
            case SHADER_DATA_TYPE::INT2:    return 2;
            case SHADER_DATA_TYPE::INT3:    return 3;
            case SHADER_DATA_TYPE::INT4:    return 4;
            case SHADER_DATA_TYPE::BOOL:    return 1;
            case SHADER_DATA_TYPE::NONE:    return 0;
        }
        return 0;
    }
};

class BufferLayout
{
public:

    BufferLayout() {}
    BufferLayout(std::vector<BufferElement>&& bufferElements)
        : m_Stride(0)
    {
        // std::move cast lvalue bufferElements to temp rValue so that m_BufferElements can Accept it 
        m_BufferElements = std::move(bufferElements);
        CalculateOffsetAndStride();
    }
    const std::vector<BufferElement>& GetBufferElements() const { return m_BufferElements;}
    // void SetBufferElements(std::vector<BufferElement>&& bufferElements) { m_BufferElements = std::move(bufferElements);} //moves bufferElements to m_bufferElements no copying is done here 
    unsigned int GetStride() const { return m_Stride;}

private:
    void CalculateOffsetAndStride()
    {
        size_t offset = 0;
        for (auto& element : m_BufferElements)
        {
            element.Offset = offset;
            offset += element.Size; 
            m_Stride += element.Size;
        }

    }
private:

    std::vector<BufferElement> m_BufferElements;
    uint32_t m_Stride;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;
    virtual void SetData(void* data, uint32_t size) = 0;
    virtual void SetBufferLayout(const BufferLayout& bufferLayout) = 0; 
    virtual const BufferLayout& GetBufferLayout() const = 0;
    virtual u32 GetVertexCount() const = 0;

    static VertexBuffer* Create(uint32_t size);
    static VertexBuffer* Create(f32* vertices, unsigned int size);
};

class IndexBuffer
{
public:
    
    virtual ~IndexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual void SetData(u32* data, u32 count);
    virtual unsigned int GetCount() const = 0; 

    static IndexBuffer* Create(u32 count);
    static IndexBuffer* Create(u32* indices, u32 count);
};
}
