#include "Bit/Renderer/Material.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace BitEngine
{
Material::Material(const std::string& name, Shader* shader)
{
    m_Name = name;
    m_Shader = shader;
    m_PropertiesDirty = true;
    m_TexturesDirty = true;
}
Material::~Material()
{
    for(auto& texture : m_Textures)
    {
        delete texture.second;
    }
    delete m_Shader;
}

void Material::SetShader(Shader* shader)
{
    if(m_Shader == shader)
    {
        BIT_LOG_WARN("You're Setting the same shader to a material");
        return;
    }
    m_Shader = shader;

}


void Material::SetFloat(const std::string& name, f32 value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::FLOAT, .Float = value };
}
void Material::SetInt(const std::string& name, i32 value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::INT, .Int1 = value };
}
void Material::SetBool(const std::string& name, b8 value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::BOOL, .value = value };
}
void Material::SetVec2(const std::string& name, const BMath::Vec2& value)
{

    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::FLOAT2, .Float2 = value };
}
void Material::SetVec3(const std::string& name, const BMath::Vec3& value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::FLOAT3, .Float3 = value };
}
void Material::SetVec4(const std::string& name, const BMath::Vec4& value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::FLOAT4, .Float4 = value };
}
void Material::SetMat4(const std::string& name, const BMath::Mat4& value)
{
    m_Properties[name] = { .Type= MATERIAL_VALUE_TYPE::FLOAT4, .Mat4 = value };
}

void Material::SetTexture(const std::string& name, Texture* texture)
{
    m_Textures[name] = texture;
}
Texture* Material::GetTexture(const std::string& name) const
{
    if(!m_Textures.contains(name))
    {
        return 0;
    }
    return m_Textures.at(name);
}
b8 Material::HasTexture(const std::string& name) const
{
    return m_Textures.contains(name);
}
void Material::RemoveTexture(const std::string& name)
{
    if(m_Textures.contains(name))
    {
        m_Textures.erase(name);
    }
}
void Material::ClearTextures()
{
    m_Textures.clear();
}

void Material::Bind() 
{
    if(!m_Shader)
    {
        return;
    }
    m_Shader->Bind();

    //ApplyRenderState
    // ApplyRenderState();

    if(m_PropertiesDirty)
    {
        ApplyProperties();
        m_PropertiesDirty = false;
    }

    if(m_TexturesDirty)
    {
        ApplyTextures();
        m_TexturesDirty = false;
    }
}
void Material::Unbind() const
{   
    if(!m_Shader)
    {
        return;
    }
    m_Shader->Unbind();
}
void Material::ApplyProperties() const
{
    for(auto& property : m_Properties)
    {
        switch(property.second.Type)
        {
        case MATERIAL_VALUE_TYPE::FLOAT:
            m_Shader->SetFloat(property.first, property.second.Float);
            break;
        case MATERIAL_VALUE_TYPE::INT:
            m_Shader->SetInt(property.first, property.second.Int1);
            break;
        case MATERIAL_VALUE_TYPE::BOOL:
            m_Shader->SetInt(property.first, property.second.value);
            break;
        case MATERIAL_VALUE_TYPE::FLOAT2:
            m_Shader->SetFloat2(property.first, property.second.Float2);
            break;
        case MATERIAL_VALUE_TYPE::FLOAT3:
            m_Shader->SetFloat3(property.first, property.second.Float3);
            break;
        case MATERIAL_VALUE_TYPE::FLOAT4:
            m_Shader->SetFloat4(property.first, property.second.Float4);
            break;
        case MATERIAL_VALUE_TYPE::MAT3:
            break;
        case MATERIAL_VALUE_TYPE::MAT4:
            m_Shader->SetMat4(property.first, property.second.Mat4);
            break;
        case MATERIAL_VALUE_TYPE::TEXTURE_2D:
            break;
        case MATERIAL_VALUE_TYPE::TEXTURE_CUBE:
            break;
        default:

            BIT_LOG_WARN("Unsupported property type for %s", property.first.c_str());
            break;
        }
    }
}
void Material::ApplyTextures() const
{
    i32 textureCount = 0;
    for (const auto& [name, texture] : m_Textures) 
    {
        if (texture && textureCount < 32) 
        { 
            texture->Bind(textureCount);
            m_Shader->SetInt(name, textureCount);
            textureCount++;
        }
    }
}

b8 Material::HasProperty(const std::string& name) const
{
    return m_Properties.contains(name);
}
MATERIAL_VALUE_TYPE Material::GetPropertyType(const std::string& name) const
{
    return m_Properties.at(name).Type;
}
std::vector<std::string> Material::GetPropertyNames() const
{
    std::vector<std::string> propertyNames;
    for(auto& property : m_Properties)
    {
        propertyNames.push_back(property.first);
    }
    return propertyNames;
}

std::vector<std::string> Material::GetTextureNames() const
{
    std::vector<std::string> textureNames;
    for(auto& texture : m_Properties)
    {
        textureNames.push_back(texture.first);
    }
    return textureNames;
}

}


