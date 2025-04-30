#pragma once

#include "Bitpch.h"

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
    size_t Offset;
    std::string AttributeName;
    uint32_t Size;
    SHADER_DATA_TYPE Type;
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

    BufferLayout(std::vector<BufferElement>&& bufferElements)
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
        unsigned int stride = 0;
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
private:
    unsigned int m_ID;
    BufferLayout* m_BufferLayout;

public:
    VertexBuffer(float* vertices, unsigned int size);
    ~VertexBuffer();
    void Bind();
    void UnBind();
    void SetBufferLayout(BufferLayout* bufferLayout) { m_BufferLayout = bufferLayout;}
    const BufferLayout* GetBufferLayout() const { return m_BufferLayout;}

private: 

};

class IndexBuffer
{
private:
    unsigned int m_ID;
    unsigned int m_Count;

public:
    
    IndexBuffer(unsigned int * indices, unsigned int count);
    void Bind() const;
    void UnBind() const;
    const unsigned int GetCount() const { return m_Count;}

private: 

};
}
