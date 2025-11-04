#pragma once 

#include "Bit/Core/Defines.h"
#include <string>
#include <vector>

namespace BitEngine
{


typedef enum 
{
    GROUND,
    SKY,
    WALL,
    COLLISION,
} TILE_LAYER_TYPE;



class TileLayer
{

public:
    TileLayer(u32 widthInTiles, u32 heightInTiles, std::string name);
    ~TileLayer() = default;

    void SetTile(u32 x, u32 y, u32 tileID);
    u32 GetTile(u32 x, u32 y);
    void ClearTile(u32 x, u32 y);
    b8 IsValidPosition(u32 x, u32 y);
    void Fill(u32 tileID);
    void Clear();
    void Resize(u32 newWidthInTiles, u32 newHeightInTiles);
private:
    std::string m_Name;
    u32 m_WidthInTiles;
    u32 m_HeightInTiles;
    std::vector<u32> m_TileData;

    u32 m_Opacity;
    b8 m_IsVisible;
    b8 m_IsLocked; // prevents editing and moving the tile from its position 
    TILE_LAYER_TYPE m_TileLayerType;

};

}

