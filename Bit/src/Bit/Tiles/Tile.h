#pragma once 
#include "Bit/Core/Defines.h"
#include <vector>

namespace BitEngine
{
class Texture;
struct Tile
{
    u32 TileID;
    b8 IsAnimated;
    u32 AnimationFrameCount; // TileIndex : AnimationFrameCount to play the animation from the tileset
    u32 Duration;
};

class TileSet
{
public:

    TileSet(); 
    ~TileSet() = default; 
    void SetTexture(Texture* texture, f32 tilesetWidth, f32 tileSetHeight, f32 tileWidth, f32 tileHeight); 
    Texture* GetTexture() { return m_Texture; }
    Tile* GetTile(u32 tileID);
    Tile* GetTileAtPosition(f32 x, f32 y);
    void CalculateTileUVs(u32 tileID, f32* UVs);
    u32 GetTileCount() { return m_RowCount * m_ColomnCount; }
    u32 GetTilesetWidth() { return m_TileSetWidth; }
    u32 GetTilesetHeight() { return m_TileSetHeight; }
private:
    std::vector<Tile> m_Tiles;
    Texture* m_Texture;
    const char* m_TexturePath;
    u32 m_TileWidth;
    u32 m_TileHeight;
    u32 m_TileSetWidth;
    u32 m_TileSetHeight;
    u32 m_BorderSpace; // border space around the tileset
    u32 m_TileGap; // gap between the tiles and the neighbors
    u32 m_ColomnCount;
    u32 m_RowCount;
};
}
