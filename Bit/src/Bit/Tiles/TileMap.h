#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Tiles/TileLayer.h"
#include <string>
#include <vector>

namespace BitEngine
{
class TileSet;
class Tile;
class TileMap
{
public:
    TileMap(u32 widthInTiles, u32 heightInTiles, TileSet* tileSet, u32 tileSize, const char* name);

    TileLayer* AddLayer(const char* name, TILE_LAYER_TYPE layerType);

    void RemoveLayer(u32 index);

    TileLayer* GetLayer(u32 index);
    TileLayer* GetActiveLayer();
    const std::vector<TileLayer>& GetAllLayers() { return m_TileLayers; };
    TileSet* GetTileSet() { return m_Tileset; }
    void SetActiveLayer(u32 index);
    u32 GetLayerCount();
    void MoveLayer(u32 fromIndex, u32 toIndex);
    void SetTile(u32 layerIndex, u32 x, u32 y, u32 tileID);
    u32 GetTile(u32 layerIndex, u32 x, u32 y);
    void SetTileOnActiveLayer(u32 x, u32 y, u32 tileID);
    u32 GetTileOnActiveLayer(u32 x, u32 y);
    u32 GetTileSize() { return m_TileSize; } 
    u32 GetWidth() { return m_WidthInTiles; } 
    u32 GetHeight() { return m_HeightInTiles; }
    const char* GetName() { return m_Name; }
private:
    const char* m_Name; 
    std::vector<TileLayer> m_TileLayers; 
    TileSet* m_Tileset; 
    u32 m_WidthInTiles; // how much tiles we will have on the screen horizontally 
    u32 m_HeightInTiles; // how much tiles we will have on the screen vertically  
    u32 m_TileSize; // single tile size uniformed currently im not supporting ununiform sizes 
    u32 m_ActiveLayerIndex; 

};
}
