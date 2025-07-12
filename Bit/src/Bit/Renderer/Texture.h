#pragma once
#include "Bitpch.h"

namespace BitEngine 
{

class Texture
{

public:
    virtual ~Texture() = default;
    virtual void Bind(unsigned int slot) const = 0;
    virtual void Unbind() const = 0;

    static Texture* Create(const char* path);
};

}
