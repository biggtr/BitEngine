#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Tiles/TileEditorState.h"
#include "Bit/Tiles/TileRenderer.h"

namespace BitEngine
{
class TileMap;
class Renderer2D;
class Camera;

class TilemapEditor
{
public:
    TilemapEditor();
    ~TilemapEditor();
    
    void Initialize(TileMap* tilemap);
    void Update(f32 deltaTime, Camera* camera);
    void Render(Renderer2D* renderer, Camera* camera);
    
    void SelectTile(u32 tileID);
    void SetTool(TileEditorTool tool);
    void SetActiveLayer(u32 layerIndex);
    void ToggleEditorMode();
    void ToggleGrid();
    
    TileEditorState* GetEditorState() { return &m_EditorState; }
    
private:
    void HandleInput(Camera* camera);
    void HandleToolInput(Camera* camera);
    void HandleKeyboardShortcuts();
    
    void PaintTile(i32 tileX, i32 tileY);
    void EraseTile(i32 tileX, i32 tileY);
    void FloodFill(i32 startX, i32 startY);
    
    void RenderCursor(Renderer2D* renderer, Camera* camera);
    
    TileEditorState m_EditorState;
    TileRenderer m_TileRenderer;
    
    BMath::Vec2 m_MouseTilePos;
    BMath::Vec2 m_LastPaintedTile;
    b8 m_IsPainting;
    b8 m_IsErasing;
};
}
