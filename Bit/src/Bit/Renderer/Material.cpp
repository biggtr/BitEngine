#include "Bit/Core/Logger.h"
#include "Bit/Resources/ResourceTypes.h"
#include "Bit/Math/Vector.h"
#include <cstdlib>
#include <cstring>
#include <vector>

#define MAX_MATERIALS_NUMBER 4096

namespace MaterialSystem
{

struct MaterialSystemState
{
    std::vector<Material> RegisteredMaterials;
    b8 initialized; 
};

static MaterialSystemState* state;
static void LoadFromFile(const char* path);

b8 Initialize()
{
    if(state->initialized)
    {
        BIT_LOG_ERROR("Material System is already initialized..!");
        return false;
    }
    state = (MaterialSystemState*)malloc(sizeof(MaterialSystemState));
    return true;
}

//TODO: change material RegisteredMaterials to ptr and change vector to my own dynamic array
Material* CreateMaterial(u32 shaderID, const char* materialName)
{
    u32 count = state->RegisteredMaterials.size();
    Material material
    {
        .ID = count,
        .InternalID = shaderID, 
        .Name = "",
        .Properties = {},
        .TextureSlots = {}
    };
    strcpy(material.Name, materialName);
    state->RegisteredMaterials.push_back(material);

    return &state->RegisteredMaterials[count];
}

void SetProperty(Material* material, const char* name, f32 value) 
{
    if(!material)
    {
        BIT_LOG_WARN("Material isnt valid..!");
        return;
    }
    material->Properties[name].Type = MATERIAL_VALUE_TYPE::FLOAT;
    material->Properties[name].float3 = value; 
}
void SetProperty(Material* material, const char* name, const BMath::Vec2& vec2) 
{
    if(!material)
    {
        BIT_LOG_WARN("Material isnt valid..!");
        return;
    }
    material->Properties[name].Type = MATERIAL_VALUE_TYPE::FLOAT2;
    material->Properties[name].float2 = vec2; 
}
void SetProperty(Material* material, const char* name, const BMath::Vec3& vec3) 
{
    if(!material)
    {
        BIT_LOG_WARN("Material isnt valid..!");
        return;
    }
    material->Properties[name].Type = MATERIAL_VALUE_TYPE::FLOAT3;
    material->Properties[name].float3 = vec3; 
}
void SetProperty(Material* material, const char* name, const BMath::Vec4& vec4) 
{
    if(!material)
    {
        BIT_LOG_WARN("Material isnt valid..!");
        return;
    }
    material->Properties[name].Type = MATERIAL_VALUE_TYPE::FLOAT4;
    material->Properties[name].float4 = vec4; 
}
MaterialProperty GetProperty(Material* material, const char* name)
{
    if(!material)
    {
        BIT_LOG_WARN("Material isnt valid..!");
    }
    if(!material->Properties.contains(name)) 
    {
        BIT_LOG_ERROR("Material Property with name %s was not found");
        return {};
    }
    return material->Properties[name];
}

void Shutdown()
{
    if(!state->initialized)
    {
        BIT_LOG_ERROR("Material System is already dead..!");
        return;
    }
    free(state);
}
}

