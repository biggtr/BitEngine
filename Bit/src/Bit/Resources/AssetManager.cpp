#include "AssetManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Texture.h"



namespace BitEngine
{
AssetManager::AssetManager()
    :   m_Textures()
    {}

void AssetManager::AddTexture(const char *assetID, const char *filePath)
{
    Texture* texture = Texture::Create(filePath);

    if(m_Textures.contains(assetID))
    {
        BIT_LOG_DEBUG("Asset with ID : %s is already there", assetID);
        return;
    }

    m_Textures[assetID] = texture;
    BIT_LOG_INFO("Texture Asset With ID: %s Added..", assetID);

}
Texture* AssetManager::GetTexture(const char *assetID)
{
    Texture* texture = nullptr;
    if(m_Textures.contains(assetID))
    {
        texture = m_Textures[assetID];
    }
    return texture;
}

void AssetManager::ClearTextures()
{
    for(const auto& pair : m_Textures)
    {
        delete pair.second;
    }
    BIT_LOG_INFO("Removed All Texture Assets");
    m_Textures.clear();
}
void AssetManager::ClearAnimations()
{

}
void AssetManager::AddAnimation(const char *assetID, const Animation2DComponent& animation)
{

}
const Animation2DComponent& GetAnimation(const char *assetID)
{
    return {};
}
}
