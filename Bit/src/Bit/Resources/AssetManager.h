#pragma once 
#include "Bit/Renderer/Texture.h"
#include "Bit/Scene/Compontents.h"
#include <map>
namespace BitEngine
{

class AssetManager
{
private:
    std::map<const char* , Texture*> m_Textures;
    std::map<const char* , CAnimation2D&> m_Animations;


public:
    AssetManager();
    ~AssetManager(){}
    void ClearTextures();
    void AddTexture(const char *assetID, const char *filePath);
    Texture* GetTexture(const char *assetID);

    void ClearAnimations();
    void AddAnimation(const char *assetID, const CAnimation2D& animation);
    const CAnimation2D& GetAnimation(const char *assetID);

};

}
