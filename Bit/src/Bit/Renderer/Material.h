#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Matrix.h"
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
enum class BLEND_MODE
{
    ALPHA,
    ADDITIVE,
    MULTIPLY
};
struct MaterialProperty
{
    MATERIAL_VALUE_TYPE Type;
    union
    {
        b8 value;
        f32 Float; 
        i32 Int1;
        BMath::Vec2 Float2;
        BMath::Vec3 Float3;
        BMath::Vec4 Float4;
        BMath::Mat4 Matrix4;
    };
};
namespace BitEngine
{

class Material
{
private:
    Shader* m_Shader;
    std::unordered_map<std::string, MaterialProperty> m_Properties;
    std::unordered_map<std::string, Texture*> m_Textures;
    std::string m_Name;
    BLEND_MODE m_BlendMode;
    f32 m_BlendStrength;

    b8 m_PropertiesDirty;
    b8 m_TexturesDirty;
public:
    Material(const std::string& name, Shader* shader);
    ~Material();
    
    void SetShader(Shader* shader);
    Shader* GetShader() const { return m_Shader; }

    
    void SetFloat(const std::string& name, f32 value);
    void SetInt(const std::string& name, i32 value);
    void SetBool(const std::string& name, b8 value);
    void SetVec2(const std::string& name, const BMath::Vec2& value);
    void SetVec3(const std::string& name, const BMath::Vec3& value);
    void SetVec4(const std::string& name, const BMath::Vec4& value);
    // void SetMat3(const std::string& name, const BMath::Mat3& value);
    void SetMat4(const std::string& name, const BMath::Mat4& value);

    void SetBlendMode(BLEND_MODE mode) { m_BlendMode = mode; }
    void SetBlendStrength(f32 strength) { m_BlendStrength = strength; }

    void SetTexture(const std::string& name, Texture* texture);
    Texture* GetTexture(const std::string& name) const;
    bool HasTexture(const std::string& name) const;
    void RemoveTexture(const std::string& name);
    void ClearTextures();

    void Bind();
    void Unbind() const;
    void ApplyProperties() const;
    void ApplyTextures() const;
    void ApplyRenderState() const; // render blending modes etc..
    
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    b8 HasProperty(const std::string& name) const;
    MATERIAL_VALUE_TYPE GetPropertyType(const std::string& name) const;
    std::vector<std::string> GetPropertyNames() const;
    std::vector<std::string> GetTextureNames() const;
private:
    void MarkPropertiesDirty() { m_PropertiesDirty = true; }
    void MarkTexturesDirty() { m_TexturesDirty = true; }
};


}


