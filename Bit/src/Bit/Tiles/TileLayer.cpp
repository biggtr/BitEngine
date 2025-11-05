#include "TileLayer.h"
#include "Bit/Core/Logger.h"
#include <cstring>


namespace BitEngine 
{
TileLayer::TileLayer(u32 widthInTiles, u32 heightInTiles, std::string name)
{

    m_WidthInTiles = widthInTiles;
    m_HeightInTiles = heightInTiles;
    m_Name = name;
    m_TileData.reserve(widthInTiles * heightInTiles);
    m_IsVisible = true;
    m_IsLocked = false;
    m_Opacity = 1.0;
}

void TileLayer::SetTile(u32 x, u32 y, u32 tileID)
{
    if(!IsValidPosition(x, y))
    {
        BIT_LOG_ERROR("Tile Position out of bound");
        return;
    }
    u32 index = y * m_WidthInTiles + x;
    m_TileData[index] = tileID;
}
u32 TileLayer::GetTile(u32 x, u32 y)
{
    u32 index = y * m_WidthInTiles + x;
    return m_TileData[index];
}
void TileLayer::ClearTile(u32 x, u32 y)
{
    u32 index = y * m_WidthInTiles + x;
    m_TileData[index] = -1;
}
b8 TileLayer::IsValidPosition(u32 x, u32 y)
{
    return x >= 0 && x < m_WidthInTiles && y >= 0 && y < m_HeightInTiles;
}
void TileLayer::Fill(u32 tileID)
{
    for(u32 i = 0; i < m_TileData.size(); ++i)
    {
        m_TileData[i]= tileID;
    }
}
void TileLayer::Clear()
{
    for(u32 i = 0; i < m_TileData.size(); ++i)
    {
        m_TileData[i]= -1;
    }
}
void TileLayer::Resize(u32 newWidthInTiles, u32 newHeightInTiles)
{
    m_TileData.resize(newWidthInTiles * newHeightInTiles);
    m_WidthInTiles = newWidthInTiles;
    m_HeightInTiles =  newWidthInTiles;
}
}
