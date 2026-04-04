#pragma once

#include "Bit/Core/Defines.h"
#include <string>

namespace BitEngine
{
class ComputeShader
{
public:
    virtual ~ComputeShader() = default;

    virtual void Dispatch(u32 workGroupSizeX, u32 workGroupSizeY, u32 workGroupSizeZ) = 0;

    virtual const char* GetPath() = 0;

    virtual u32 GetID() = 0;
    static ComputeShader* Create(const char* path);

};
}
