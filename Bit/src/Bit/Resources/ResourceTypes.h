#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/VertexArray.h"
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

    MaterialProperty() {}
    MaterialProperty(f32 v) : Type(MATERIAL_VALUE_TYPE::FLOAT), value(v) {}
    MaterialProperty(BMath::Vec2& v) : Type(MATERIAL_VALUE_TYPE::FLOAT2), float2(v) {}
    MaterialProperty(BMath::Vec3& v) : Type(MATERIAL_VALUE_TYPE::FLOAT3), float3(v) {}
    MaterialProperty(BMath::Vec4& v) : Type(MATERIAL_VALUE_TYPE::FLOAT4), float4(v) {}
};

#define MAX_MATERIAL_NAME_LENGTH 256
struct Material
{
    u32 ID;
    u32 InternalID; // id of the shader in the renderer
    char Name[MAX_MATERIAL_NAME_LENGTH];
    std::unordered_map<const char*, MaterialProperty>Properties;
    std::unordered_map<const char*, u32> TextureSlots;
};

