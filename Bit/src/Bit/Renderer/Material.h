#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Resources/ResourceTypes.h"
#include <unordered_map>
#include <vector>


#define MAX_MATERIAL_PROPERTY_NAME_LENGTH 127
#define MAX_MATERIAL_PROPERTIES 128

b8 MaterialSystemInitialize(u64* memoryRequirement, void* state);
Material* CreateMaterial(u32 shaderID, const char* materialName);
void MaterialSystemSetProperty(Material* material, const char* name, f32 value);
void MaterialSystemSetProperty(Material* material, const char* name, const BMath::Vec2& vec2);
void MaterialSystemSetProperty(Material* material, const char* name, const BMath::Vec3& vec3);
void MaterialSystemSetProperty(Material* material, const char* name, const BMath::Vec4& vec4);
MaterialProperty MaterialSystemGetProperty(Material* material, const char* name);
void Shutdown();


