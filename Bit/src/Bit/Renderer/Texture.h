#pragma once
#include "Bit/Core/Defines.h"
#include "Bitpch.h"
#include <cstdint>

namespace BitEngine 
{

class Texture
{

public:
    virtual ~Texture() = default;
    virtual u32 GetID() const = 0;
    virtual void Bind(unsigned int slot = 0) const = 0;
    virtual void Unbind() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void SetData(void* data, uint32_t size) = 0;

    static Texture* Create(const char* path);
    static Texture* Create(uint32_t width, uint32_t height);
};

}
