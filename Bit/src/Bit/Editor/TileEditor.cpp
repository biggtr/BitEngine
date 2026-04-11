#include "TileEditor.h"
#include "Bit/Core/Defines.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Resources/AssetStore.h"
#include "Bit/Tiles/Tile.h"
#include "Bit/Tiles/TileEditorState.h"
#include "Bit/Tiles/TileLayer.h"
#include "Bit/Tiles/TileMap.h"
#include "Bit/Tiles/TileRenderer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace BitEngine
{

TileEditor::TileEditor(Renderer2D* renderer2D)
    :   m_TileMap(nullptr),
        m_TileSet(nullptr),
        m_EditorState(),
        m_TileRenderer(renderer2D),
        m_Renderer2D(renderer2D),
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

TileSet* TileEditor::CreateTileSet(Texture* texture, u32 tilesetWidth, u32 tilesetHeight, u32 tileWidth, u32 tileHeight)
{
    if (m_TileSet)
    {
        BIT_LOG_WARN("tileset is already there");
        return m_TileSet;
    }
    
    m_TileSet = new TileSet();
    BIT_LOG_INFO("Created new tileset");
    SetTileSetTexture(texture, tilesetWidth, tilesetHeight, tileWidth, tileHeight);
    return m_TileSet;
}

void TileEditor::SetTileSetTexture(Texture* texture, u32 tilesetWidth, u32 tilesetHeight, u32 tileWidth, u32 tileHeight)
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


// widthInTiles means how many tiles do you want the map have horizontally 
// heightInTiles means how many tiles do you want the map have vertically
TileMap* TileEditor::CreateTileMap(const char* name, u32 widthInTiles, u32 heightInTiles, u32 tileSize)
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
    m_TileMap->GetTileSize();
    m_EditorState.SetTileMap(m_TileMap);

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


void TileEditor::AddLayer(const char* name, TILE_LAYER_TYPE type)
{
    if (!m_TileMap)
    {
        BIT_LOG_ERROR("Cannot add layer: no tilemap exists");
        return;
    }
    
    m_TileMap->AddLayer(name, type);
    BIT_LOG_INFO("Added layer: %s", name);
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
    ControlEditorCamera(camera);

    if (m_IsPainting || m_IsErasing)
    {
        HandleToolInput(camera);
    }
}
void TileEditor::ControlEditorCamera(Camera* camera)
{
    BMath::Vec3 cameraMovement = BMath::Vec3Zero();
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D))
    {
        cameraMovement.x = 1.0f;
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_A))
    {
        cameraMovement.x = -1.0f;
    }
    if(BitEngine::InputIsKeyDown(KEY_W))
    {
        cameraMovement.y = 1.0f;
    }
    if(BitEngine::InputIsKeyDown(KEY_S))
    {
        cameraMovement.y = -1.0f;
    }
    f32 cameraSpeed = 2.0f;
    BMath::Vec3 cameraPos = camera->GetPosition();
    camera->SetPosition(cameraPos + (cameraMovement * cameraSpeed));
}
b8 TileEditor::IsTileSolid(i32 tileX, i32 tileY, u32 layerIndex)
{
    if(!m_TileMap)
        return false;

    if(layerIndex >= m_TileMap->GetLayerCount())
        return false;

    i32 halfW = (i32)(m_TileMap->GetWidth()  / 2);
    i32 halfH = (i32)(m_TileMap->GetHeight() / 2);
    if(tileX < -halfW || tileX >= halfW) return false;
    if(tileY < -halfH || tileY >= halfH) return false;

    TileLayer* layer = m_TileMap->GetLayer(layerIndex);
    if(!layer || !layer->IsVisible() || layer->GetType() != TILE_LAYER_TYPE::COLLISION)
        return false;

    u32 tileID = layer->GetTile(tileX, tileY);
    return tileID != 0;
}

void TileEditor::Render(const BMath::Mat4& viewProjection)
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
        RenderCursor(m_Renderer2D, viewProjection);
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
    m_EditorState.SetCameraEditor(!currentMode);
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
    
    renderer->DrawRect(position, size, 0.0f, cursorColor);
    
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
    
    
    u32 selectedTile = m_EditorState.GetSelectedTile();
    layer->SetTile(tileX, tileY, selectedTile);
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
    
    
    layer->SetTile(tileX, tileY, 0);
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

b8 TileEditor::SaveTileMap(char* path)
{
    strcat(path, ".bmap");
    FILE* f = fopen(path, "wb");
    if(f == nullptr)
    {
        BIT_LOG_WARN("Couldn't Create new file to save TileMap");
        return false;
    }
    TileSet* tileSet = m_TileMap->GetTileSet();

    const char* texturePath = tileSet->GetTexture()->GetPath();
    u32 texturePathLen = strlen(texturePath);
    fwrite(&texturePathLen, sizeof(u32), 1, f);
    fwrite(texturePath, sizeof(char), texturePathLen, f);

    const char* textureName = tileSet->GetTexture()->GetName();
    u32 textureNameLen = strlen(textureName);
    fwrite(&textureNameLen, sizeof(u32), 1, f);
    fwrite(textureName, sizeof(char), textureNameLen, f);

    u32 tilesetWidth = tileSet->GetTilesetWidth();
    u32 tilesetHeight = tileSet->GetTilesetHeight();
    u32 singleTileWidth = tileSet->GetSingleTileWidth();
    u32 singleTileHeight = tileSet->GetSingleTileHeight();
    fwrite(&tilesetWidth, sizeof(u32), 1, f);
    fwrite(&tilesetHeight, sizeof(u32), 1, f);
    fwrite(&singleTileWidth, sizeof(u32), 1, f);
    fwrite(&singleTileHeight, sizeof(u32), 1, f);

    TileMap* tileMap = m_TileMap;
    u32 nTilesOnScreenWidth = tileMap->GetWidth();
    u32 nTilesOnScreenHeight = tileMap->GetHeight();
    u32 singleTileSize = tileMap->GetTileSize();
    const char* tileMapName = tileMap->GetName();
    u32 tileMapNameLen = strlen(tileMapName);
    fwrite(&nTilesOnScreenWidth, sizeof(u32), 1, f);
    fwrite(&nTilesOnScreenHeight, sizeof(u32), 1, f);
    fwrite(&singleTileSize, sizeof(u32), 1, f);
    fwrite(&tileMapNameLen, sizeof(u32), 1, f);
    fwrite(tileMapName, sizeof(char), tileMapNameLen, f);

    std::vector<TileLayer> tileLayers = tileMap->GetAllLayers();
    u32 tileLayersCount = tileLayers.size();
    fwrite(&tileLayersCount, sizeof(u32), 1, f);
    for(u32 i = 0; i < tileLayersCount; ++i)
    {
        std::vector<u32> tileData = tileLayers[i].GetTileData();
        u32 tileDataCount = tileData.size();
        fwrite(&tileDataCount, sizeof(u32), 1, f);
        fwrite(tileData.data(), sizeof(u32), tileDataCount, f);
        
        const char* layerName = tileLayers[i].GetName();
        u32 layerNameLen = strlen(layerName);
        fwrite(&layerNameLen, sizeof(u32), 1, f);
        fwrite(layerName, sizeof(char), layerNameLen, f);
    }
    fclose(f);
    return true;
}

b8 TileEditor::LoadTileMap(char* path)
{
    strcat(path, ".bmap");
    FILE* f = fopen(path, "rb");

    if(f == nullptr)
    {
        BIT_LOG_ERROR("Couldn't open the file %s", path);
        return false;
    }
    u32 texturePathLen;
    fread(&texturePathLen, sizeof(u32), 1, f);
    char* texturePath = (char*)malloc(sizeof(char) * texturePathLen + 1);
    fread(texturePath, sizeof(char), texturePathLen, f);
    texturePath[texturePathLen] = '\0';



    u32 textureNameLen;
    fread(&textureNameLen, sizeof(u32), 1, f);
    char* textureName = (char*)malloc(sizeof(char) * textureNameLen + 1);
    fread(textureName, sizeof(char), textureNameLen, f);
    textureName[textureNameLen] = '\0';
    BIT_LOG_INFO("Loading texture '%s' from: %s", textureName, texturePath);

    Texture* texture = AssetStoreAddTexture(textureName, texturePath);
    if (texture == nullptr)
    {
        BIT_LOG_ERROR("Failed to load texture: %s from path: %s", textureName, texturePath);
        free(textureName);
        free(texturePath);
        fclose(f);
        return false;
    }
    free(textureName);
    free(texturePath);

    u32 tilesetWidth;
    u32 tilesetHeight;
    u32 singleTileWidth;
    u32 singleTileHeight;
    fread(&tilesetWidth, sizeof(u32), 1, f);
    fread(&tilesetHeight, sizeof(u32), 1, f);
    fread(&singleTileWidth, sizeof(u32), 1, f);
    fread(&singleTileHeight, sizeof(u32), 1, f);
    TileSet* tileSet = CreateTileSet(texture, tilesetWidth, tilesetHeight, singleTileWidth, singleTileHeight);
    if (!tileSet)
    {
        BIT_LOG_ERROR("Failed to create tileset");
        delete texture;
        fclose(f);
        return false;
    }

    u32 nTilesOnScreenWidth;
    u32 nTilesOnScreenHeight;
    u32 singleTileSize;
    u32 tileMapNameLen;
    fread(&nTilesOnScreenWidth, sizeof(u32), 1, f);
    fread(&nTilesOnScreenHeight, sizeof(u32), 1, f);
    fread(&singleTileSize, sizeof(u32), 1, f);
    fread(&tileMapNameLen, sizeof(u32), 1, f);

    char* tileMapName = (char*)malloc(sizeof(char) * tileMapNameLen + 1);
    fread(tileMapName, sizeof(char), tileMapNameLen, f);
    tileMapName[tileMapNameLen] = '\0';
    if (m_TileMap)
    {
        delete m_TileMap;
        m_TileMap = nullptr;
    }
    m_TileMap = new TileMap(nTilesOnScreenWidth, nTilesOnScreenHeight, tileSet, singleTileSize, tileMapName);
    m_EditorState.SetTileMap(m_TileMap);
    free(tileMapName);

    u32 tileLayersCount;
    fread(&tileLayersCount, sizeof(u32), 1, f);
    for(u32 i = 0; i < tileLayersCount; ++i)
    {
        u32 tileDataCount;
        fread(&tileDataCount, sizeof(u32), 1, f);

        std::vector<u32> tileData(tileDataCount);
        fread(tileData.data(), sizeof(u32), tileDataCount, f);

        u32 layerNameLen;
        fread(&layerNameLen, sizeof(u32), 1, f);
        
        char* layerName = (char*)malloc(layerNameLen + 1);
        fread(layerName, sizeof(char), layerNameLen, f);
        layerName[layerNameLen] = '\0';

        AddLayer(layerName, TILE_LAYER_TYPE::COLLISION);
        TileLayer* layer = m_TileMap->GetLayer(i);
        if(layer) 
        {
            layer->SetTileData(tileData); 
        }
        free(layerName);
    }

    fclose(f);
    return true;
}
}
