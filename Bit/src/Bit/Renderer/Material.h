#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Renderer/Texture.h"
#include <unordered_map>
#include <vector>


#define MAX_MATERIAL_PROPERTY_NAME_LENGTH 127
#define MAX_MATERIAL_PROPERTIES 128

enum class MATERIAL_VALUE_TYPE
{
    FLOAT,
    INT,
    BOOL,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MAT3,
    MAT4,
    TEXTURE_2D,
    TEXTURE_CUBE
};
struct MaterialProperty
{
    MATERIAL_VALUE_TYPE Type;
    union
    {
        f32 Float1; 
        BMath::Vec2 Float2;
        BMath::Vec3 Float3;
        BMath::Vec4 Float4;
    };

    MaterialProperty() {}
    MaterialProperty(f32 float1) : Type(MATERIAL_VALUE_TYPE::FLOAT), Float1(float1) {}
    MaterialProperty(BMath::Vec2& v) : Type(MATERIAL_VALUE_TYPE::FLOAT2), Float2(v) {}
    MaterialProperty(BMath::Vec3& v) : Type(MATERIAL_VALUE_TYPE::FLOAT3), Float3(v) {}
    MaterialProperty(BMath::Vec4& v) : Type(MATERIAL_VALUE_TYPE::FLOAT4), Float4(v) {}
};
namespace BitEngine
{

class Material
{
private:
    Shader* m_Shader;
    std::unordered_map<std::string, MaterialProperty> m_Properties;
    std::unordered_map<std::string, Texture*> m_Textures;

    bool m_PropertiesDirty = true;
    bool m_TexturesDirty = true;
public:
    void Bind() const;
    
    void SetFloat(const std::string& name, f32 value);
    void SetInt(const std::string& name, i32 value);
    void SetBool(const std::string& name, bool value);
    void SetVec2(const std::string& name, const BMath::Vec2& value);
    void SetVec3(const std::string& name, const BMath::Vec3& value);
    void SetVec4(const std::string& name, const BMath::Vec4& value);
    // void SetMat3(const std::string& name, const BMath::Mat3& value);
    void SetMat4(const std::string& name, const BMath::Mat4& value);

};

}


