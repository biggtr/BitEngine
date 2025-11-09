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

    void SetTile(i32 x, i32 y, u32 tileID);
    u32  GetTile(i32 x, i32 y);
    void ClearTile(i32 x, i32 y);

    void Fill(u32 tileID);
    void Clear();
    void Resize(u32 newWidthInTiles, u32 newHeightInTiles);

    void SetLayerType(TILE_LAYER_TYPE tileLayerType) { m_TileLayerType = tileLayerType; }
    TILE_LAYER_TYPE GetType() { return m_TileLayerType; }
    void SetVisible(b8 visible){ m_IsVisible = visible; }
    b8 IsVisible(){ return m_IsVisible; }
    u32 GetWidth() { return m_WidthInTiles; }
    u32 GetHeight() { return m_HeightInTiles; }

private:
    u32 GetIndex(i32 x, i32 y);

    std::string m_Name;
    u32 m_WidthInTiles;
    u32 m_HeightInTiles;
    std::vector<u32> m_TileData;

    u32 m_Opacity;
    b8 m_IsVisible;
    b8 m_IsLocked;
    TILE_LAYER_TYPE m_TileLayerType;
};

}
