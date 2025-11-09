#pragma once 
#include "Bit/Renderer/Texture.h"
#include "Bit/ECS/Compontents.h"
#include <map>
namespace BitEngine
{

class AssetManager
{
private:
    std::map<const char* , Texture*> m_Textures;
    std::map<const char* , Animation2DComponent&> m_Animations;


public:
    AssetManager();
    ~AssetManager(){}
    void ClearTextures();
    Texture* AddTexture(const char *assetID, const char *filePath);
    Texture* GetTexture(const char *assetID);

    void ClearAnimations();
    void AddAnimation(const char *assetID, const Animation2DComponent& animation);
    const Animation2DComponent& GetAnimation(const char *assetID);

};

}
