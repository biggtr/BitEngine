#include "TileMap.h"
#include "Bit/Core/Logger.h"
#include "Tile.h"

namespace BitEngine
{

TileMap::TileMap(u32 widthInTiles, u32 heightInTiles, TileSet* tileSet, u32 tileSize, const std::string& name)
{
    m_WidthInTiles = widthInTiles;
    m_HeightInTiles = heightInTiles;
    m_Tileset = tileSet;
    m_TileSize = tileSize;
    m_Name = name;
    m_ActiveLayerIndex = 0;
}
TileLayer* TileMap::AddLayer(const std::string& name, TILE_LAYER_TYPE layerType)
{
    TileLayer tileLayer(m_WidthInTiles, m_HeightInTiles, name);
    tileLayer.SetLayerType(layerType);
    m_TileLayers.push_back(tileLayer);
    return &m_TileLayers.back();
}

void TileMap::RemoveLayer(u32 index)
{
    if(index > m_TileLayers.size())
    {
        BIT_LOG_ERROR("index %d is out of bound", index);
        return; 
    }
    TileLayer layer(0, 0, "none");
    m_TileLayers[index] = layer;
}

TileLayer* TileMap::GetLayer(u32 index)
{
    if(index > m_TileLayers.size())
    {
        BIT_LOG_ERROR("index %d is out of bound", index);
        return nullptr;
    }
    return &m_TileLayers[index];
}
TileLayer* TileMap::GetActiveLayer()
{
    return &m_TileLayers[m_ActiveLayerIndex];
}
void TileMap::SetActiveLayer(u32 index)
{
    m_ActiveLayerIndex = index;
}
u32  TileMap::GetLayerCount()
{
    return m_TileLayers.size();
}
void TileMap::MoveLayer(u32 fromIndex, u32 toIndex)
{
    TileLayer temp = m_TileLayers[toIndex];
    m_TileLayers[toIndex] = m_TileLayers[fromIndex];
    m_TileLayers[fromIndex] = temp;
}
void TileMap::SetTile(u32 layerIndex, u32 x, u32 y, u32 tileID)
{
    TileLayer layer = m_TileLayers[layerIndex];
    layer.SetTile(x, y, tileID);
}
u32 TileMap::GetTile(u32 layerIndex, u32 x, u32 y)
{
    TileLayer layer = m_TileLayers[layerIndex];
    return layer.GetTile(x, y);
}
void TileMap::SetTileOnActiveLayer(u32 x, u32 y, u32 tileID)
{
    TileLayer layer = m_TileLayers[m_ActiveLayerIndex];
    layer.SetTile(x, y, tileID);
}
u32 TileMap::GetTileOnActiveLayer(u32 x, u32 y)
{
    TileLayer layer = m_TileLayers[m_ActiveLayerIndex];
    return layer.GetTile(x, y);
}
}
