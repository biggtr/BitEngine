#include "TileEditor.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Tiles/Tile.h"
#include "Bit/Tiles/TileMap.h"
#include "Bit/Tiles/TileRenderer.h"
#include <processthreadsapi.h>

namespace BitEngine
{

TileEditor::TileEditor(Renderer2D* renderer2D)
    :   m_TileMap(nullptr),
        m_TileSet(nullptr),
        m_EditorState(),
        m_TileRenderer(renderer2D),
        m_MouseTilePos(),
        m_LastPaintedTile(),
        m_IsPainting(false),
        m_IsErasing(false)
{
}

TileEditor::~TileEditor()
{
    if (m_TileMap)
        delete m_TileMap;
    if (m_TileSet)
        delete m_TileSet;
}
void TileEditor::Initialize()
{
    m_EditorState.SetActiveTool(TileEditorTool::PAINT);
    m_EditorState.SetSelectedTile(0);
    m_EditorState.SetActiveLayer(0);
    m_EditorState.SetEditorMode(false);
    
    m_IsPainting = false;
    m_IsErasing = false;
    m_MouseTilePos = BMath::Vec3Zero();
    m_LastPaintedTile = BMath::Vec3Zero();
    
    
    BIT_LOG_INFO("TileEditor initialized");
}

TileSet* TileEditor::CreateTileSet(Texture* texture, f32 tilesetWidth, f32 tilesetHeight, f32 tileWidth, f32 tileHeight)
{
    if (m_TileSet)
    {
        BIT_LOG_WARN("Deleting existing tileset");
        delete m_TileSet;
    }
    
    m_TileSet = new TileSet();
    BIT_LOG_INFO("Created new tileset");
    SetTileSetTexture(texture, tilesetWidth, tilesetHeight, tileWidth, tileHeight);
    return m_TileSet;
}

void TileEditor::SetTileSetTexture(Texture* texture, f32 tilesetWidth, f32 tilesetHeight, f32 tileWidth, f32 tileHeight)
{
    if (!m_TileSet)
    {
        BIT_LOG_ERROR("Cannot set texture: no tileset exists. Call CreateTileSet() first");
        return;
    }
    
    m_TileSet->SetTexture(texture, tilesetWidth, tilesetHeight, tileWidth, tileHeight);
    BIT_LOG_INFO("Tileset texture set: %.0fx%.0f, tile size: %.0fx%.0f", 
                 tilesetWidth, tilesetHeight, tileWidth, tileHeight);
}

TileMap* TileEditor::CreateTileMap(const std::string& name, u32 widthInTiles, u32 heightInTiles, u32 tileSize)
{
    if (!m_TileSet)
    {
        BIT_LOG_ERROR("Cannot create tilemap: no tileset exists. Call CreateTileSet() first");
        return nullptr;
    }
    
    if (m_TileMap)
    {
        BIT_LOG_WARN("Deleting existing tilemap");
        delete m_TileMap;
    }
    
    m_TileMap = new TileMap(widthInTiles, heightInTiles, m_TileSet, tileSize, name);
    m_EditorState.SetTileMap(m_TileMap);
    BIT_LOG_INFO("Created new tilemap: %s (%dx%d tiles, tile size: %d)", 
                 name.c_str(), widthInTiles, heightInTiles, tileSize);

    return m_TileMap;
}
void TileEditor::ClearTileMap()
{
    if (!m_TileMap)
        return;
    
    i32 mapWidth = (i32)m_TileMap->GetWidth();
    i32 mapHeight = (i32)m_TileMap->GetHeight();

    i32 minX = -mapWidth / 2;
    i32 minY = -mapHeight / 2;
    i32 maxX = minX + mapWidth;
    i32 maxY = minY + mapHeight;

    for (u32 i = 0; i < m_TileMap->GetLayerCount(); ++i)
    {
        TileLayer* layer = m_TileMap->GetLayer(i);
        if (layer)
        {
            for (i32 y = minY; y < maxY; ++y)
            {
                for (i32 x = minX; x < maxX; ++x)
                {
                    layer->SetTile(x, y, 0);
                }
            }
        }
    }
    
    BIT_LOG_INFO("Cleared tilemap");
}


void TileEditor::AddLayer(const std::string& name, TILE_LAYER_TYPE type)
{
    if (!m_TileMap)
    {
        BIT_LOG_ERROR("Cannot add layer: no tilemap exists");
        return;
    }
    
    m_TileMap->AddLayer(name, type);
    BIT_LOG_INFO("Added layer: %s", name.c_str());
}

void TileEditor::RemoveLayer(u32 index)
{
    if (!m_TileMap)
        return;
    
    if (index < m_TileMap->GetLayerCount())
    {
        m_TileMap->RemoveLayer(index);
        BIT_LOG_INFO("Removed layer at index: %d", index);
        
        u32 currentLayer = m_EditorState.GetActiveLayer();
        if (currentLayer >= m_TileMap->GetLayerCount() && m_TileMap->GetLayerCount() > 0)
        {
            SetActiveLayer(m_TileMap->GetLayerCount() - 1);
        }
    }
}

void TileEditor::MoveLayerUp(u32 index)
{
    if (!m_TileMap)
        return;
    
    if (index > 0 && index < m_TileMap->GetLayerCount())
    {
        m_TileMap->MoveLayer(index, index - 1);
        BIT_LOG_INFO("Moved layer %d up", index);
    }
}

void TileEditor::MoveLayerDown(u32 index)
{
    if (!m_TileMap)
        return;
    
    if (index < m_TileMap->GetLayerCount() - 1)
    {
        m_TileMap->MoveLayer(index, index + 1);
        BIT_LOG_INFO("Moved layer %d down", index);
    }
}

void TileEditor::SetLayerVisible(u32 index, b8 visible)
{
    if (!m_TileMap)
        return;
    
    if (index < m_TileMap->GetLayerCount())
    {
        TileLayer* layer = m_TileMap->GetLayer(index);
        if (layer)
        {
            layer->SetVisible(visible);
            BIT_LOG_INFO("Layer %d visibility: %s", index, visible ? "ON" : "OFF");
        }
    }
}

u32 TileEditor::GetLayerCount()
{
    if (!m_TileMap)
        return 0;
    return m_TileMap->GetLayerCount();
}

void TileEditor::Update(f32 deltaTime, Camera* camera, const BMath::Mat4& viewProjection)
{
    if (!InputWasKeyDown(KEY_E) && InputIsKeyDown(KEY_E))
    {
        ToggleEditorMode();
    }
    if (!m_EditorState.IsEditorMode())
        return;
    
    if (!m_EditorState.GetTileMap())
        return;
    
    HandleKeyboardShortcuts();
    HandleInput(camera, viewProjection);
    
    if (m_IsPainting || m_IsErasing)
    {
        HandleToolInput(camera);
    }
}

b8 TileEditor::IsTileSolid(i32 tileX, i32 tileY, u32 layerIndex)
{
    if(!m_TileMap)
        return false;

    if(layerIndex >= m_TileMap->GetLayerCount())
        return false;

    TileLayer* layer = m_TileMap->GetLayer(layerIndex);
    if(!layer || !layer->IsVisible())
        return false;

    u32 tileID = layer->GetTile(tileX, tileY);

    return tileID != 0;
}
void TileEditor::GetTileCollisions(const BMath::Vec3& position, f32 width, f32 height, std::vector<TileCollisionInfo>& collisions, u32 layerIndex)
{
    if (!m_TileMap)
        return;
    
    collisions.clear();
    BMath::Vec3 centerPosition = position;
    centerPosition.x += width * 0.5f;
    centerPosition.y += height * 0.5f;
    u32 tileSize = m_TileMap->GetTileSize();
    i32 minTileX = BMath::Floor((centerPosition.x - width * 0.5f) / tileSize);
    i32 maxTileX = BMath::Floor((centerPosition.x + width * 0.5f) / tileSize);
    i32 minTileY = BMath::Floor((centerPosition.y - height * 0.5f) / tileSize);
    i32 maxTileY = BMath::Floor((centerPosition.y + height * 0.5f) / tileSize);

    for(i32 ty = minTileY; ty < maxTileY; ++ty)
    {
        for(i32 tx = minTileX; tx < maxTileX; ++tx)
        {
            if (!IsTileSolid(tx, ty, layerIndex))
                continue;

            f32 tileWorldX = (f32)tx * (f32)tileSize + (f32)tileSize * 0.5f;
            f32 tileWorldY = (f32)ty * (f32)tileSize + (f32)tileSize * 0.5f;

            //distance between center of entity and tile
            f32 dx = position.x - tileWorldX;
            f32 dy = position.y - tileWorldY;

            f32 combinedHalfWidth = (width + tileSize) * 0.5f;
            f32 combinedHalfHeight = (height + tileSize) * 0.5f;

            //if position from center to center is bigger than the combinedHalfWidth or combinedHalfHeight this means no overlap no collision
            f32 overlapX = combinedHalfWidth - abs(dx);
            f32 overlapY = combinedHalfHeight - abs(dx);

            if(overlapX > 0.0f && overlapY > 0.0f)
            {
                TileCollisionInfo collisionInfo;
                collisionInfo.IsColliding = true;
                collisionInfo.TileX = tx;
                collisionInfo.TileY = ty;

                if(overlapX < overlapY)
                {
                    collisionInfo.Depth = overlapX;
                    collisionInfo.Normal = BMath::Vec3(dx > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
                }
                else
                {
                    collisionInfo.Depth = overlapY;
                    collisionInfo.Normal = BMath::Vec3(0.0f, dy > 0.0f ? 1.0f : -1.0f, 0.0f);
                }
                collisions.push_back(collisionInfo);
            }
        }

    }
}
void TileEditor::Render(Renderer2D* renderer, const BMath::Mat4& viewProjection)
{
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    m_TileRenderer.Render(tilemap, viewProjection);
    
    if (m_EditorState.ShowGrid())
    {
        BMath::Vec4 gridColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_TileRenderer.RenderGrid(tilemap, viewProjection, gridColor, tilemap->GetTileSize());
    }
    
    if (m_EditorState.IsEditorMode())
    {
        RenderCursor(renderer, viewProjection);
    }
}

void TileEditor::SelectTile(u32 tileID)
{
    m_EditorState.SetSelectedTile(tileID);
}
void TileEditor::SetTool(TileEditorTool tool)
{
    m_EditorState.SetActiveTool(tool);

    const char* toolName = "NONE";
    switch (tool) 
    {
        case TileEditorTool::PAINT: toolName = "PAINT"; break;
        case TileEditorTool::ERASE: toolName = "ERASE"; break;
        case TileEditorTool::FILL: toolName = "FILL"; break;
        default: break;
    }
}
void TileEditor::SetActiveLayer(u32 layerIndex)
{
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (tilemap && layerIndex < tilemap->GetLayerCount())
    {
        m_EditorState.SetActiveLayer(layerIndex);
        BIT_LOG_DEBUG("Active layer: %d", layerIndex);
    }
}
void TileEditor::ToggleEditorMode()
{
    b8 currentMode = m_EditorState.IsEditorMode();
    m_EditorState.SetEditorMode(!currentMode);
    BIT_LOG_DEBUG("Editor mode: %s", !currentMode ? "ON" : "OFF");
}
void TileEditor::ToggleGrid()
{
    m_EditorState.ToggleGrid();
    BIT_LOG_DEBUG("Grid: %s", m_EditorState.ShowGrid() ? "ON" : "OFF");
}
void TileEditor::HandleInput(Camera* camera, const BMath::Mat4& viewProjection)
{
    i32 mouseX, mouseY;
    InputGetMousePosition(&mouseX, &mouseY);
    
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    if (m_ScreenWidth == 0 || m_ScreenHeight == 0)
    {
        BIT_LOG_ERROR("Screen dimensions not set");
        return;
    }
    
    mouseX = BMath::Clamp(mouseX, 0, (i32)m_ScreenWidth - 1);
    mouseY = BMath::Clamp(mouseY, 0, (i32)m_ScreenHeight - 1);
    
    const BMath::Mat4 invVP = BMath::Mat4Inverse(viewProjection);
    
    f32 ndcX = (2.0f * mouseX) / m_ScreenWidth - 1.0f;
    f32 ndcY = 1.0f - (2.0f * mouseY) / m_ScreenHeight;
    
    BMath::Vec4 worldPos = invVP * BMath::Vec4(ndcX, ndcY, 0.0f, 1.0f);
    
    u32 tileSize = tilemap->GetTileSize();

    m_MouseTilePos.x = BMath::Floor(worldPos.x / (f32)tileSize);
    m_MouseTilePos.y = BMath::Floor(worldPos.y / (f32)tileSize);
    
    
    if (InputIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_IsPainting = true;
        m_LastPaintedTile = BMath::Vec3(-1000.0f, -1000.0f, 0.0f);
    }
    
    if (InputIsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        m_IsPainting = false;
    }
    
    if (InputIsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        m_IsErasing = true;
    }
    
    if (InputIsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        m_IsErasing = false;
    }
}

void TileEditor::RenderCursor(Renderer2D* renderer, const BMath::Mat4& viewProjection)
{
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    i32 tileX = (i32)m_MouseTilePos.x;
    i32 tileY = (i32)m_MouseTilePos.y;
    
    u32 tileSize = tilemap->GetTileSize();
    i32 mapWidth = (i32)tilemap->GetWidth();
    i32 mapHeight = (i32)tilemap->GetHeight();
    
    i32 minX = -(mapWidth / 2);
    i32 minY = -(mapHeight / 2);
    i32 maxX = minX + mapWidth;
    i32 maxY = minY + mapHeight;
    
    if (tileX < minX - 1 || tileX >= maxX + 1 || 
        tileY < minY - 1 || tileY >= maxY + 1)
    {
        return;
    }
    
    f32 worldX = (f32)tileX * (f32)tileSize + (f32)tileSize * 0.5f;
    f32 worldY = (f32)tileY * (f32)tileSize + (f32)tileSize * 0.5f;
    
    BMath::Vec3 position(worldX, worldY, 0.2f);
    BMath::Vec3 size((f32)tileSize, (f32)tileSize, 1.0f);
    
    
    BMath::Vec4 cursorColor;
    switch (m_EditorState.GetActiveTool())
    {
        case TileEditorTool::PAINT:
            cursorColor = BMath::Vec4(0.0f, 1.0f, 0.0f, 0.3f); 
            break;
        case TileEditorTool::ERASE:
            cursorColor = BMath::Vec4(1.0f, 0.0f, 0.0f, 0.3f); 
            break;
        case TileEditorTool::FILL:
            cursorColor = BMath::Vec4(0.0f, 0.5f, 1.0f, 0.3f); 
            break;
        default:
            cursorColor = BMath::Vec4(1.0f, 1.0f, 1.0f, 0.3f); 
            break;
    }
    
    renderer->DrawRect(position, size, cursorColor);
    
    BMath::Vec4 borderColor(cursorColor.x, cursorColor.y, cursorColor.z, 0.8f);
    f32 halfSize = (f32)tileSize * 0.5f;
    f32 borderZ = 0.3f;
    
    renderer->DrawLine(
        BMath::Vec3(worldX - halfSize, worldY + halfSize, borderZ),
        BMath::Vec3(worldX + halfSize, worldY + halfSize, borderZ),
        borderColor
    );
    
    renderer->DrawLine(
        BMath::Vec3(worldX - halfSize, worldY - halfSize, borderZ),
        BMath::Vec3(worldX + halfSize, worldY - halfSize, borderZ),
        borderColor
    );
    
    renderer->DrawLine(
        BMath::Vec3(worldX - halfSize, worldY - halfSize, borderZ),
        BMath::Vec3(worldX - halfSize, worldY + halfSize, borderZ),
        borderColor
    );
    
    renderer->DrawLine(
        BMath::Vec3(worldX + halfSize, worldY - halfSize, borderZ),
        BMath::Vec3(worldX + halfSize, worldY + halfSize, borderZ),
        borderColor
    );
}

void TileEditor::PaintTile(i32 tileX, i32 tileY)
{
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    u32 layerIndex = m_EditorState.GetActiveLayer();
    if (layerIndex >= tilemap->GetLayerCount())
        return;
    
    TileLayer* layer = tilemap->GetLayer(layerIndex);
    if (!layer)
        return;
    
    i32 mapWidth = (i32)tilemap->GetWidth();
    i32 mapHeight = (i32)tilemap->GetHeight();
    i32 localX = tileX + (mapWidth / 2);
    i32 localY = tileY + (mapHeight / 2);
    
    if (localX < 0 || localX >= mapWidth || localY < 0 || localY >= mapHeight)
        return;
    
    u32 selectedTile = m_EditorState.GetSelectedTile();
    layer->SetTile(localX, localY, selectedTile);
}

void TileEditor::EraseTile(i32 tileX, i32 tileY)
{
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    u32 layerIndex = m_EditorState.GetActiveLayer();
    if (layerIndex >= tilemap->GetLayerCount())
        return;
    
    TileLayer* layer = tilemap->GetLayer(layerIndex);
    if (!layer)
        return;
    
    i32 mapWidth = (i32)tilemap->GetWidth();
    i32 mapHeight = (i32)tilemap->GetHeight();
    i32 localX = tileX + (mapWidth / 2);
    i32 localY = tileY + (mapHeight / 2);
    
    if (localX < 0 || localX >= mapWidth || localY < 0 || localY >= mapHeight)
        return;
    
    layer->SetTile(localX, localY, 0);
}
void TileEditor::HandleToolInput(Camera* camera)
{
    i32 tileX = (i32)m_MouseTilePos.x;
    i32 tileY = (i32)m_MouseTilePos.y;
    
    TileMap* tilemap = m_EditorState.GetTileMap();
    if (!tilemap)
        return;
    
    
    TileEditorTool tool = m_EditorState.GetActiveTool();
    
    switch (tool)
    {
        case TileEditorTool::PAINT:
        {
            if (m_IsPainting)
            {
                if (m_MouseTilePos.x != m_LastPaintedTile.x || 
                    m_MouseTilePos.y != m_LastPaintedTile.y)
                {
                    PaintTile(tileX, tileY);
                    m_LastPaintedTile = m_MouseTilePos;
                }
            }
            break;
        }
        
        case TileEditorTool::ERASE:
        {
            if (m_IsErasing)
            {
                EraseTile(tileX, tileY);
            }
            break;
        }
        
        case TileEditorTool::FILL:
        {
            static b8 fillTriggered = false;
            if (m_IsPainting && !fillTriggered)
            {
                FloodFill(tileX, tileY);
                fillTriggered = true;
            }
            if (!m_IsPainting)
            {
                fillTriggered = false;
            }
            break;
        }
        
        default:
            break;
    }
}
void TileEditor::HandleKeyboardShortcuts()
{

    if (!InputWasKeyDown(KEY_1) && InputIsKeyDown(KEY_1))
    {
        SetTool(TileEditorTool::PAINT);
    }
    if (!InputWasKeyDown(KEY_2) && InputIsKeyDown(KEY_2))
    {
        SetTool(TileEditorTool::ERASE);
    }
    if (!InputWasKeyDown(KEY_3) && InputIsKeyDown(KEY_3))
    {
        SetTool(TileEditorTool::FILL);
    }
    
    if (!InputWasKeyDown(KEY_G) && InputIsKeyDown(KEY_G))
    {
        ToggleGrid();
    }
    
    
    if (InputIsKeyDown(KEY_LSHIFT) || InputIsKeyDown(KEY_RSHIFT))
    {
        if (!InputWasKeyDown(KEY_UP) && InputIsKeyDown(KEY_UP))
        {
            u32 currentLayer = m_EditorState.GetActiveLayer();
            TileMap* tilemap = m_EditorState.GetTileMap();
            if (tilemap && currentLayer < tilemap->GetLayerCount() - 1)
            {
                SetActiveLayer(currentLayer + 1);
            }
        }
        if (!InputWasKeyDown(KEY_DOWN) && InputIsKeyDown(KEY_DOWN))
        {
            u32 currentLayer = m_EditorState.GetActiveLayer();
            if (currentLayer > 0)
            {
                SetActiveLayer(currentLayer - 1);
            }
        }
    }
}

void TileEditor::FloodFill(i32 startX, i32 startY)
{
}

void TileEditor::SetScreenSize(u32 screenWidth, u32 screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

}
