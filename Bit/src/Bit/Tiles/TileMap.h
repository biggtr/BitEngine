#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Tiles/TileLayer.h"
#include <vector>
#include <string.h>

namespace BitEngine
{
class TileSet;

class TileMap
{
public:
void AddLayer(const std::string& name, TILE_LAYER_TYPE layerType);

void RemoveLayer(u32 index);

void GetLayer(u32 index);
u32 GetActiveLayer();
void SetActiveLayer(u32 index);
u32 GetLayerCount();
void MoveLayer(u32 fromIndex, u32 toIndex);
void SetTile(u32 layerIndex, u32 x, u32 y, u32 tileID);
void GetTile(u32 layerIndex, u32 x, u32 y);
void SetTileOnActiveLayer(u32 x, u32 y, u32 tileID);
void GetTileOnActiveLayer(u32 x, u32 y);
void GetMapWidthInPixels();

private:
    std::string m_Name; 
    std::vector<TileLayer> m_Layers; 
    TileSet* m_Tileset; 
    u32 m_WidthInTiles; 
    u32 m_HeightInTiles; 
    u32 m_TileCount; 
    u32 m_ActiveLayerIndex; 

};
}
