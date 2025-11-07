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

void TileRenderer::Render(TileMap* tileMap, Camera* camera2D)
{
    for(u32 i = 0; i < tileMap->GetLayerCount(); ++i)
    {
        TileLayer* layer = tileMap->GetLayer(i);
        TileSet* tileSet = tileMap->GetTileSet(); 
        if(!layer->IsVisible())
            continue;
        if(layer->GetType() == TILE_LAYER_TYPE::COLLISION)
            continue;

        RenderLayer(layer, tileSet, camera2D, tileMap->GetTileSize());
    }
}

void TileRenderer::RenderGrid(TileMap* tileMap, Camera* camera2D, const BMath::Vec4& gridColor, u32 tileSize)
{
    VisibleBounds bounds = CalculateVisibleBounds(camera2D, tileMap->GetWidth(), tileMap->GetHeight(), tileSize);

    //Drawing Vertical line
    for(i32 x = bounds.MinTileX; x <= bounds.MaxTileX + 1; ++x)
    {
        f32 worldX = x * tileSize;
        f32 startY = bounds.MinTileY * tileSize;
        f32 endY   = (bounds.MaxTileY + 1) * tileSize;

        m_Renderer2D->DrawLine(
                BMath::Vec3(worldX, startY, -4.5f),
                BMath::Vec3(worldX, endY, -4.5f),
                gridColor
            );
    }

    //Drawing Horizontal line
    for(i32 y = bounds.MinTileY; y <= bounds.MaxTileY + 1; ++y)
    {
        f32 worldY = y * tileSize;
        f32 startX = bounds.MinTileX * tileSize;
        f32 endX   = (bounds.MaxTileX + 1) * tileSize;

        m_Renderer2D->DrawLine(
                BMath::Vec3(worldY, startX, -4.5f),
                BMath::Vec3(worldY, endX, -4.5f),
                gridColor
            );
    }
}

void TileRenderer::RenderLayer(TileLayer* tileLayer, TileSet* tileSet, Camera* camera2D, u32 tileSize)
{
    VisibleBounds bounds = CalculateVisibleBounds(camera2D, tileLayer->GetWidth(), tileLayer->GetHeight(), tileSize);

    for(i32 y = bounds.MinTileY; y < bounds.MaxTileY; ++y)
    {
        for(i32 x = bounds.MinTileX; x < bounds.MaxTileX; ++x)
        {
            u32 tileID = tileLayer->GetTile(x, y);

            if (tileID == (u32)-1 || tileID == 0)
                continue;
            
            Tile* tile = tileSet->GetTile(tileID);
            if (!tile)
                continue;

            BMath::Vec3 position;
            position.x = x * tileSize;
            position.y = y * tileSize;
            position.z = -5.0f;  // adjsut this in future
            
            BMath::Vec3 size(tileSize, tileSize, 1.0f);


            f32 Uvs[8] = {};
            tileSet->CalculateTileUVs(tileID, Uvs);
            m_Renderer2D->DrawQuad(position, size, {}, tileSet->GetTexture(), Uvs);
        }
    }

}

VisibleBounds TileRenderer::CalculateVisibleBounds(Camera* camera, u32 mapWidth, u32 mapHeight, u32 tileSize)
{
    VisibleBounds bound;
    BMath::Mat4 invVP = BMath::Mat4Inverse(camera->GetViewMatrix());
    
    //Getting the world coords from the ndc
    BMath::Vec4 topLeft = invVP * BMath::Vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 topRight = invVP * BMath::Vec4(1.0f, 1.0f, 0.0f, 1.0f);
    BMath::Vec4 botRight = invVP * BMath::Vec4(1.0f, -1.0f, 0.0f, 1.0f);
    BMath::Vec4 botLeft = invVP * BMath::Vec4(-1.0f, -1.0f, 0.0f, 1.0f);

    bound.Left = BMath::Min(topLeft.x, botLeft.x);
    bound.Right = BMath::Min(topRight.x, botRight.x);
    bound.Top = BMath::Min(topLeft.y, topRight.y);
    bound.Bottom = BMath::Min(botLeft.y, botRight.y);

    //Getting bound in tile coords 
    bound.MinTileX = BMath::Max(0.0f, (i32)BMath::Floor(bound.Left / tileSize));
    bound.MaxTileX = BMath::Min(mapWidth - 1, (i32)BMath::Ceil(bound.Right / tileSize));
    bound.MinTileY = BMath::Max(0.0f, (i32)BMath::Floor(bound.Top / tileSize));
    bound.MaxTileY = BMath::Min(mapHeight - 1, (i32)BMath::Ceil(bound.Bottom / tileSize));

    return bound;
}
}
