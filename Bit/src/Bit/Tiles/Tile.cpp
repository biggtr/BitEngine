#include "Tile.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Texture.h"

namespace BitEngine
{

TileSet::TileSet()
{

}

void TileSet::SetTexture(Texture* texture, f32 tilesetWidth, f32 tileSetHeight, f32 tileWidth, f32 tileHeight)
{
    m_Texture = texture;
    m_TileSetWidth = tilesetWidth;
    m_TileSetHeight = tileSetHeight;
    m_TileWidth = tileWidth;
    m_TileHeight = tileHeight;

    m_ColomnCount = m_TileSetWidth / m_TileWidth;
    m_RowCount = m_TileSetHeight / m_TileHeight;

    //Assigning index for each tile in the tileset as an index to access it in future
    m_Tiles.reserve(GetTileCount());
    for(u32 i = 0; i < GetTileCount(); ++i)
    {
        Tile tile = {}; 
        tile.TileID = i;
        m_Tiles.push_back(tile);
    }
}

Tile* TileSet::GetTile(u32 tileID)
{
    if(tileID >= m_Tiles.size())
    {
        BIT_LOG_ERROR("Tile id %d is not found in the tileset array ", tileID);
        return nullptr;
    }
    return &m_Tiles[tileID];
}
Tile* TileSet::GetTileAtPosition(f32 x, f32 y)
{
    u32 tileX = x / m_TileWidth ;
    u32 tileY = y / m_TileHeight;
    u32 tileID = tileY * m_ColomnCount + tileX;

    return &m_Tiles[tileID];
}
void TileSet::CalculateTileUVs(u32 tileID, f32* UVs)
{

    const u8 x = tileID % m_ColomnCount;
    const u8 y = tileID / m_ColomnCount;

    float u1,v1,u2,v2;

    u1 = (float)x / m_ColomnCount; 
    v1 = (float)y / m_RowCount;
    u2 = u1 + (1.0f / m_ColomnCount);
    v2 = v1 + (1.0f / m_RowCount);

        
    UVs[0] = u1; UVs[1] = v1;
    UVs[2] = u2; UVs[3] = v1;
    UVs[4] = u2; UVs[5] = v2;
    UVs[6] = u1; UVs[7] = v2;

}

}
