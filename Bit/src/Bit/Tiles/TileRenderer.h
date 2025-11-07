#pragma once
#include "Bit/Tiles/TileEditorState.h"
#include "Bit/Math/Vector.h"

namespace BitEngine
{

class Renderer2D;
class Camera;
class TileSet;
class TileLayer;
struct VisibleBounds;


class TileRenderer
{
public:
    TileRenderer(Renderer2D* renderer2D);
    ~TileRenderer();

    void Render(TileMap* tileMap, Camera* camera2D);
    void RenderGrid(TileMap* tileMap, Camera* camera2D, const BMath::Vec4& gridColor, u32 tileSize);

private:
    void RenderLayer(TileLayer* tileLayer, TileSet* tileSet, Camera* camera2D, u32 tileSize);

private:
    Renderer2D* m_Renderer2D;
    VisibleBounds CalculateVisibleBounds(Camera* camera, u32 mapWidth, u32 mapHeight, u32 tileSize);
};
}
