#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/Texture.h"
#include "Bit/Tiles/TileEditorState.h"
#include "Bit/Tiles/TileLayer.h"
#include "Bit/Tiles/TileRenderer.h"
#include <string>

namespace BitEngine
{
class TileMap;
class Renderer2D;
class Camera;

class TileEditor
{
public:
    TileEditor(Renderer2D* renderer2D);
    ~TileEditor(); 
    
    void Initialize();
    void Update(f32 deltaTime, Camera* camera, const BMath::Mat4& viewProjection);
    void Render(Renderer2D* renderer, const BMath::Mat4& viewProjection);
    
    TileSet* CreateTileSet(Texture* texture, f32 tilesetWidth, f32 tilesetHeight, f32 tileWidth, f32 tileHeight);
    void SetTileSetTexture(Texture* texture, f32 tilesetWidth, f32 tilesetHeight, 
                           f32 tileWidth, f32 tileHeight);
    TileSet* GetTileSet() { return m_TileSet; }
    
    TileMap* CreateTileMap(const std::string& name, u32 widthInTiles, 
                          u32 heightInTiles, u32 tileSize);
    TileMap* GetTileMap() { return m_TileMap; }
    void ClearTileMap();
    
    void AddLayer(const std::string& name, TILE_LAYER_TYPE type);
    void RemoveLayer(u32 index);
    void MoveLayerUp(u32 index);
    void MoveLayerDown(u32 index);
    void SetLayerVisible(u32 index, b8 visible);
    u32 GetLayerCount();
    
    b8 SaveTileMap(const std::string& filepath);
    b8 LoadTileMap(const std::string& filepath);

    void SelectTile(u32 tileID);
    void SetTool(TileEditorTool tool);
    void SetActiveLayer(u32 layerIndex);
    void ToggleEditorMode();
    void ToggleGrid();
    void SetScreenSize(u32 screenWidth, u32 screenHeight);
    TileEditorState* GetEditorState() { return &m_EditorState; }
    
private:
    void HandleInput(Camera* camera, const BMath::Mat4& viewProjection);
    void HandleToolInput(Camera* camera);
    void HandleKeyboardShortcuts();
    
    void PaintTile(i32 tileX, i32 tileY);
    void EraseTile(i32 tileX, i32 tileY);
    void FloodFill(i32 startX, i32 startY);
    
    void RenderCursor(Renderer2D* renderer, const BMath::Mat4& viewProjection);

    u32 m_ScreenWidth;
    u32 m_ScreenHeight;
    
    TileMap* m_TileMap;
    TileSet* m_TileSet;
    TileEditorState m_EditorState;
    TileRenderer m_TileRenderer;
    
    BMath::Vec3 m_MouseTilePos;
    BMath::Vec3 m_LastPaintedTile;
    b8 m_IsPainting;
    b8 m_IsErasing;
};
}
