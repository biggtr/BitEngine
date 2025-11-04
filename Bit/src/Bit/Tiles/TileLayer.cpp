#include "TileLayer.h"


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

}
