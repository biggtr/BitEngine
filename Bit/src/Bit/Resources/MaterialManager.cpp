#include "MaterialManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Material.h"

namespace BitEngine
{

MaterialManager::MaterialManager(ShaderManager* shaderManager)
{
    m_ShaderManager = shaderManager;

    CreateDefaultMaterial();
}
MaterialManager::~MaterialManager()
{
    Clear();
}
    
Material* MaterialManager::CreateMaterial(const std::string& name, const std::string& shaderName)
{
    if(HasMaterial(name))
    {
        BIT_LOG_WARN("Material %s already exists, returning existing", name.c_str());
        return GetMaterial(name);
    }
    Shader* shader = m_ShaderManager->GetShader(shaderName);
    if(!shader)
    {
        BIT_LOG_ERROR("Failed To Create Material %s Shader %s not found", name.c_str(), shaderName.c_str());
        return m_DefaultMaterial;
    }
    return CreateMaterial(name, shader);
}
Material* MaterialManager::CreateMaterial(const std::string& name, Shader* shader)
{
    if(HasMaterial(name))
    {
        BIT_LOG_WARN("Material %s already exists, returning existing", name.c_str());
        return GetMaterial(name);
    }
    if(!shader)
    {
        BIT_LOG_ERROR("Failed To Create Material %s Shader %s not found", name.c_str(), shader->GetName().c_str());
        return m_DefaultMaterial;
    }
    Material* material = new Material(name, shader);
    m_Materials[name] = material;
    return m_Materials[name];
}

// Material* MaterialManager::CreateUnlitMaterial(const std::string& name, const BMath::Vec4& color = BMath::Vec4(1.0f, 0, 0, 1.0f)); // no lights
// Material* MaterialManager::CreateLitMaterial(const std::string& name,
//             const BMath::Vec3& diffuse = BMath::Vec3(0.8f),
//             const BMath::Vec3& specular = BMath::Vec3(0.2f),
//             f32 shininess = 32.0f); 

// Material* MaterialManager::CreateTextureMaterial(const std::string& name, Texture* texture);
    
Material* MaterialManager::GetMaterial(const std::string& name)
{
    if(!HasMaterial(name))
    {
        BIT_LOG_WARN("Material %s doesnt exist, ", name.c_str());
        return m_DefaultMaterial;
    }
    return m_Materials[name];
}
b8 MaterialManager::HasMaterial(const std::string& name) const
{
    return m_Materials.contains(name);
}
b8 MaterialManager::RemoveMaterial(const std::string& name)
{
    if(!HasMaterial(name))
    {
        BIT_LOG_WARN("Material %s doesnt exist, ", name.c_str());
        return false;
    }
    return m_Materials.erase(name);
}
void MaterialManager::Clear()
{
    for(auto& material : m_Materials)
    {
        delete material.second;
    }
    delete m_DefaultMaterial;
}
    
// std::vector<std::string> MaterialManager::GetMaterialNames() const;

// void MaterialManager::LoadBuiltinMaterials()
// {
//
// }

void MaterialManager::CreateDefaultMaterial()
{
    if(!m_DefaultMaterial)
        m_DefaultMaterial = CreateMaterial("default", "defaultShader");
}
}
