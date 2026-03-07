#include "AssetStore.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Memory/BMemory.h"
#include "Bit/Renderer/Texture.h"
#include <cstring>
#include <map>

#define MAX_TEXTURES 512


namespace BitEngine
{
struct AssetStoreState
{
    Texture* Textures[MAX_TEXTURES];
    u32 TextureCount;
};

static AssetStoreState* statePtr = 0;


b8 AssetsStoreInitialize(u64* memoryRequirement, void* state)
{
    *memoryRequirement = sizeof(AssetStoreState);
    if(state == nullptr)
    {
        return true;
    }
    statePtr = (AssetStoreState*)state;
    memset(statePtr, 0, sizeof(AssetStoreState));
    return true;
}
b8 AssetStoreShutdown(void* state)
{
    if(state)
    {
        memset(statePtr, 0, sizeof(AssetStoreState));
        statePtr = 0;        
    }
    BIT_LOG_INFO("Asset Manager Is shutting down..!");
    return true;
}
Texture* AssetStoreAddTexture(const char *assetID, const char *filePath)
{
    if(statePtr->TextureCount >= MAX_TEXTURES)
    {
        BIT_LOG_ERROR("Texture Store is full!!");
    }
    for(u32 i = 0; i < statePtr->TextureCount; ++i)
    {
        if(strcmp(statePtr->Textures[i]->GetName(), assetID) == 0)
        {
            BIT_LOG_DEBUG("Asset with ID : %s is already there", assetID);
            return statePtr->Textures[i];
        }
    }
    Texture* texture = Texture::Create(assetID, filePath);
    statePtr->Textures[statePtr->TextureCount] = texture;
    statePtr->TextureCount++;
    BIT_LOG_INFO("Texture Asset With ID: %s Added..", assetID);
    return statePtr->Textures[statePtr->TextureCount - 1];
}
Texture* AssetStoreGetTexture(const char *assetID)
{
    for(u32 i = 0; i < statePtr->TextureCount; i++)
    {
        if(strcmp(statePtr->Textures[i]->GetName(), assetID) == 0)
        {
            return statePtr->Textures[i];
        }
    }
    BIT_LOG_WARN("Texture not found: %s", assetID);
    return 0;
}

void AssetStoreClearTextures()
{
    for(u32 i = 0; i < statePtr->TextureCount; ++i)
    {
        delete statePtr->Textures[i];
    }
    BIT_LOG_INFO("Removed All Texture Assets");
}
}
