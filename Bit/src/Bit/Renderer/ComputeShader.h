#pragma once

#include "Bit/Core/Defines.h"
#include <string>

namespace BitEngine
{
class ComputeShader
{
public:
    virtual ~ComputeShader() = default;

    virtual void Dispatch() = 0;

    virtual const std::string& GetName() = 0;
    virtual void SetName(const std::string& name) = 0;

    virtual u32 GetID() = 0;
    static ComputeShader* Create(const std::string& path);

};
}
