#pragma once 
#include "Bit/Renderer/Texture.h"
#include <map>
namespace BitEngine
{

class AssetManager
{
private:
    std::map<const char* , Texture*> m_Textures;


public:
    AssetManager();
    ~AssetManager(){}
    void ClearTextures();
    void AddTexture(const char *assetID, const char *filePath);
    Texture* GetTexture(const char *assetID);


};

}
