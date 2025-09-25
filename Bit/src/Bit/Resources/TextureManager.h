#pragma once 

#include "Bit/Renderer/Texture.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace BitEngine
{

class TextureManager
{

private:
    std::unordered_map<std::string, Texture*> m_Textures;
    Texture* m_DefaultTexture;
    
public:
    TextureManager();
    ~TextureManager();
    
    Texture* CreateTexture(const std::string& name, const std::string& filepath);
    Texture* CreateTexture(const std::string& name, Texture* texture);
    
    Texture* GetTexture(const std::string& name);
    b8 HasTexture(const std::string& name) const;
    b8 RemoveTexture(const std::string& name);
    void Clear();
    
    std::vector<std::string> GetTextureNames() const;
    u32 GetTextureCount() const { return (u32)(m_Textures.size()); }
    
    void LoadBuiltinTextures();
    
private:
    void CreateDefaultTexture();
};
}
