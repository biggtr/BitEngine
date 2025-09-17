#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include <unordered_map>



enum class MATERIAL_VALUE_TYPE
{
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4
};
struct MaterialProperty
{
    MATERIAL_VALUE_TYPE Type;
    union
    {
        f32 value; 
        BMath::Vec2 float2;
        BMath::Vec3 float3;
        BMath::Vec4 float4;
    };
};
struct Material
{
    u32 ID;
    u32 ShaderID;
    const char* Name;
    std::unordered_map<const char*, MaterialProperty>Properties;
    std::unordered_map<const char*, u32> TextureSlots;
};
