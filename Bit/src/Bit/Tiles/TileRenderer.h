#pragma once
#include "Bit/Math/Matrix.h"
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

    void Render(TileMap* tileMap, const BMath::Mat4& viewProjection);
    void RenderGrid(TileMap* tileMap, const BMath::Mat4& viewProjection, const BMath::Vec4& gridColor, u32 tileSize);

private:
    void RenderLayer(TileLayer* tileLayer, TileSet* tileSet, const BMath::Mat4& viewProjection, u32 tileSize);

private:
    Renderer2D* m_Renderer2D;
    VisibleBounds CalculateVisibleBounds(const BMath::Mat4& viewProjection, u32 mapWidth, u32 mapHeight, u32 tileSize);

};
}
