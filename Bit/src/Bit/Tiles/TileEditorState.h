#pragma once

#include "Bit/Core/Defines.h"
namespace BitEngine
{
class TileMap;
class Tileset;
enum class TileEditorTool
{
    PAINT,
    ERASE,
    FILL,
    NONE
};
class TileEditorState
{

public:
    TileEditorState() = default;
    ~TileEditorState() = default;

    void SetTileMap(TileMap* tilemap) { m_ActiveTileMap = tilemap; }
    void SetSelectedTile(u32 tileID) { m_SelectedTile = tileID; }
    void SetActiveTool(TileEditorTool tool) { m_ActiveTool = tool; }
    void SetActiveLayer(u32 layerIndex) { m_ActiveLayer = layerIndex; }

    TileMap* GetTileMap() { return m_ActiveTileMap; }
    u32 GetSelectedTile() { return m_SelectedTile; }
    TileEditorTool GetActiveTool() { return m_ActiveTool; }
    u32 GetActiveLayer() { return m_ActiveLayer; }

    b8 IsEditorMode() { return m_IsEditorMode; }
    void SetEditorMode(b8 enabled) { m_IsEditorMode = enabled; }

    b8 ShowGrid() { return m_ShowGrid; }
    void ToggleGrid() { m_ShowGrid = !m_ShowGrid; }
private:
    TileMap* m_ActiveTileMap;
    u32  m_SelectedTile;
    TileEditorTool m_ActiveTool;
    u32  m_ActiveLayer;

    b8 m_IsEditorMode;
    b8 m_ShowGrid;
};

}
