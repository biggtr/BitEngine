#pragma once 
#include "Bit/Renderer/Texture.h"
#include "Bit/ECS/Compontents.h"
namespace BitEngine
{


b8 AssetsStoreInitialize(u64* memoryRequirement, void* state);
b8 AssetStoreShutdown(void* state);

Texture* AssetStoreAddTexture(const char *assetID, const char *filePath);
Texture* AssetStoreGetTexture(const char *assetID);
void AssetStoreClearTextures();

    // void ClearAnimations();
    // void AddAnimation(const char *assetID, const Animation2DComponent& animation);
    // const Animation2DComponent& GetAnimation(const char *assetID);

};

