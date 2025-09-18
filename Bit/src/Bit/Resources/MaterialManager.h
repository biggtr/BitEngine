#pragma once

#include "Bit/Renderer/Material.h"
#include "Bit/Resources/ShaderManager.h"
#include <string>
#include <unordered_map>
namespace BitEngine
{

class MaterialManager
{
private:
    std::unordered_map<std::string, Material*> m_Materials;
    ShaderManager* m_ShaderManager;
    
public:
    MaterialManager(ShaderManager* shaderManager);
    ~MaterialManager();
    
    Material* CreateMaterial(const std::string& name, const std::string& shaderName);
    Material* CreateMaterial(const std::string& name, Shader* shader);
    
    Material* CreateTextureMaterial(const std::string& name, Texture* texture);
    
    Material* GetMaterial(const std::string& name);
    bool HasMaterial(const std::string& name) const;
    bool RemoveMaterial(const std::string& name);
    void Clear();
    
    std::vector<std::string> GetMaterialNames() const;
    u32 GetMaterialCount() const { return (u32)(m_Materials.size()); }
    
    void LoadBuiltinMaterials();
    
private:
    void CreateDefaultMaterial();

};
}
