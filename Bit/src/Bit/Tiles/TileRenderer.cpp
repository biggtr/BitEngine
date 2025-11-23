#include "TileRenderer.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Tiles/Tile.h"
#include "Bit/Tiles/TileLayer.h"
#include "Bit/Tiles/TileMap.h"
#include "Bit/Renderer/Camera.h"

namespace BitEngine
{

struct VisibleBounds
{
    i32 MinTileX, MaxTileX, MinTileY, MaxTileY;
    f32 Left, Right, Top, Bottom;
};

TileRenderer::TileRenderer(Renderer2D* renderer2D)
{
    m_Renderer2D = renderer2D;
}

TileRenderer::~TileRenderer()
{
    m_Renderer2D = 0;
}

void TileRenderer::Render(TileMap* tileMap, const BMath::Mat4& viewProjection)
{
    for(u32 i = 0; i < tileMap->GetLayerCount(); ++i)
    {
        TileLayer* layer = tileMap->GetLayer(i);
        TileSet* tileSet = tileMap->GetTileSet(); 
        if(!layer->IsVisible())
            continue;
        if(layer->GetType() == TILE_LAYER_TYPE::COLLISION)
            continue;

        RenderLayer(layer, tileSet, viewProjection, tileMap->GetTileSize());
    }
}
void TileRenderer::RenderGrid(TileMap* tileMap, const BMath::Mat4& viewProjection, const BMath::Vec4& gridColor, u32 tileSize)
{
    BMath::Mat4 invVP = BMath::Mat4Inverse(viewProjection);
    
    BMath::Vec4 topLeft = invVP * BMath::Vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 topRight = invVP * BMath::Vec4(1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 botRight = invVP * BMath::Vec4(1.0f, -1.0f, 0.0f, 1.0f);
    BMath::Vec4 botLeft = invVP * BMath::Vec4(-1.0f, -1.0f, 0.0f, 1.0f);

    f32 viewLeft = BMath::Min(topLeft.x, botLeft.x);
    f32 viewRight = BMath::Max(topRight.x, botRight.x);
    f32 viewTop = BMath::Max(topLeft.y, topRight.y);
    f32 viewBottom = BMath::Min(botLeft.y, botRight.y);

    f32 gridZ = 0.1f; 
    f32 fTileSize = (f32)tileSize;

    f32 startX = BMath::Floor(viewLeft / fTileSize) * fTileSize;
    f32 endX = BMath::Ceil(viewRight / fTileSize) * fTileSize;
    f32 startY = BMath::Floor(viewBottom / fTileSize) * fTileSize;
    f32 endY = BMath::Ceil(viewTop / fTileSize) * fTileSize;

    for(f32 x = startX; x <= endX; x += fTileSize)
    {
        m_Renderer2D->DrawLine(
            BMath::Vec3(x, viewBottom, gridZ),
            BMath::Vec3(x, viewTop, gridZ),   
            gridColor
        );
    }
    
    for(f32 y = startY; y <= endY; y += fTileSize)
    {
        m_Renderer2D->DrawLine(
            BMath::Vec3(viewLeft, y, gridZ),  
            BMath::Vec3(viewRight, y, gridZ), 
            gridColor
        );
    }
}
void TileRenderer::RenderLayer(TileLayer* tileLayer, TileSet* tileSet, const BMath::Mat4& viewProjection, u32 tileSize)
{
    VisibleBounds bounds = CalculateVisibleBounds(viewProjection, tileLayer->GetWidth(), tileLayer->GetHeight(), tileSize);
    
    for(i32 y = bounds.MinTileY; y <= bounds.MaxTileY; ++y)
    {
        for(i32 x = bounds.MinTileX; x <= bounds.MaxTileX; ++x)
        {
            u32 tileID = tileLayer->GetTile(x, y);

            if (tileID == (u32)-1 || tileID == 0)
                continue;
            
            
            
            Tile* tile = tileSet->GetTile(tileID);
            if (!tile)
            {
                continue;
            }

            BMath::Vec3 position;
            position.x = (f32)x * (f32)tileSize + (f32)tileSize * 0.5f; 
            position.y = (f32)y * (f32)tileSize + (f32)tileSize * 0.5f;
            position.z = -0.5f;


            BMath::Vec3 size((f32)tileSize, (f32)tileSize, 1.0f);

            f32 Uvs[8] = {};
            tileSet->CalculateTileUVs(tileID, Uvs);
            m_Renderer2D->DrawQuad(position, size, {}, tileSet->GetTexture(), Uvs);
        }
    }
}
VisibleBounds TileRenderer::CalculateVisibleBounds(const BMath::Mat4& viewProjection, u32 mapWidth, u32 mapHeight, u32 tileSize)
{
    VisibleBounds bound;
    BMath::Mat4 invVP = BMath::Mat4Inverse(viewProjection);
    
    BMath::Vec4 topLeft = invVP * BMath::Vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 topRight = invVP * BMath::Vec4(1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 botRight = invVP * BMath::Vec4(1.0f, -1.0f, 0.0f, 1.0f);
    BMath::Vec4 botLeft = invVP * BMath::Vec4(-1.0f, -1.0f, 0.0f, 1.0f);

    bound.Left = BMath::Min(topLeft.x, botLeft.x);
    bound.Right = BMath::Max(topRight.x, botRight.x);
    bound.Top = BMath::Max(topLeft.y, topRight.y);
    bound.Bottom = BMath::Min(botLeft.y, botRight.y);

    bound.MinTileX = (i32)BMath::Floor(bound.Left / tileSize);
    bound.MaxTileX = (i32)BMath::Ceil(bound.Right / tileSize);
    bound.MinTileY = (i32)BMath::Floor(bound.Bottom / tileSize);
    bound.MaxTileY = (i32)BMath::Ceil(bound.Top / tileSize);

    
    return bound;
}

}
