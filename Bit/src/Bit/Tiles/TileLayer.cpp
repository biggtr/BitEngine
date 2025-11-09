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
    m_TileData.resize(widthInTiles * heightInTiles, 0); 
    m_IsVisible = true;
    m_IsLocked = false;
    m_Opacity = 1.0;
}
u32 TileLayer::GetIndex(i32 x, i32 y)
{
    i32 offsetX = x + (m_WidthInTiles / 2);
    i32 offsetY = y + (m_HeightInTiles / 2);

    if (offsetX < 0 || offsetX >= m_WidthInTiles || offsetY < 0 || offsetY >= m_HeightInTiles)
    {
        return (u32)-1; 
    }
    
    return offsetX + offsetY * m_WidthInTiles;
}
void TileLayer::SetTile(i32 x, i32 y, u32 tileID)
{
    u32 index = GetIndex(x, y);
    if (index != (u32)-1) 
    {
        m_TileData[index] = tileID;
    }
}
u32 TileLayer::GetTile(i32 x, i32 y)
{
    u32 index = GetIndex(x, y);
    if (index != (u32)-1) 
    {
        return m_TileData[index];
    }
    return 0;
}

void TileLayer::ClearTile(i32 x, i32 y)
{
    SetTile(x, y, 0); 
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
        m_TileData[i]= 0;
    }
}
void TileLayer::Resize(u32 newWidthInTiles, u32 newHeightInTiles)
{
    m_TileData.resize(newWidthInTiles * newHeightInTiles);
    m_WidthInTiles = newWidthInTiles;
    m_HeightInTiles = newHeightInTiles; 
}
}
