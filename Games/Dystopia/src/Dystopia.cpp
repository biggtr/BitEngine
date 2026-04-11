#include "Dystopia.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Font/Font.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/System.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Resources/AssetStore.h"
#include "Bit/Tiles/Tile.h"
#include "Bit/Tiles/TileLayer.h"
#include "Bit/UI/BitUI.h"
#include "Bit/UI/Widgets.h"
#include "Platform/Platform.h"
#include "PlayerController.h"
#include <cstdio>
#include <cstring>

b8 drawUI = false;
void Dystopia::Initialize()
{
    // BitEngine::PlatformHideCursor();
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::ORTHO);
    TileIndex = 0;
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();
    player.AddComponent<BitEngine::SpriteComponent>();
    player.AddComponent<Character2DControllerComponent>();

    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    playerSprite.Color = {0.4, 0.2, 0.2, 1.0f};
    
    f32 width = 10;
    f32 height = 10;
    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 100.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {width, height, 1.0f};

    auto& playerRigidBody = player.AddComponent<BitEngine::Rigidbody2DComponent>();
    playerRigidBody.Position = {0.0f, 100.0f, 0.0f};
    playerRigidBody.Type  = BitEngine::PhysicsBodyType::Kinematic;

    m_Physics2DSystem->CreateBoxShape(player, width, height, 0.0f, true);



    

    BitEngine::Texture* tilesetTexture = BitEngine::AssetStoreAddTexture("tileset", "assets/textures/spritesheet.png");
    m_TileEditor->CreateTileSet(tilesetTexture, 7 * 32, 6 * 32, 32, 32);
    m_TileEditor->SelectTile(TileIndex);


    m_TileEditor->CreateTileMap("Level_1", 1000, 1000, 5);
    m_TileEditor->AddLayer("background", BitEngine::TILE_LAYER_TYPE::COLLISION); 
    m_TileEditor->SetActiveLayer(0);

    // FontInitialize("assets/fonts/dejavu.ttf");
}

void Dystopia::RenderUI()
{
    if(drawUI)
    {
        UIBeginFrame();
        BitEngine::Texture* tileTexture = m_TileEditor->GetTileSet()->GetTexture();
        u32 tileCount = m_TileEditor->GetTileSet()->GetTileCount();
        f32 uvs[8] = {}; 
        f32 y = 100; 
        f32 x = 100;
        for(u32 i = 0; i < tileCount; ++i)
        {
            m_TileEditor->GetTileSet()->CalculateTileUVs(i, uvs);
            ButtonStyle b = {.Texture = tileTexture, .UVs = uvs};
            x = 100 + 100 * (i % 5);
            y += i % 5 == 0 ? 100 : 0;
            if(UIButton(i, {.x= x, .y = y, .w = 40, .h = 40}, b))
            {
                TileIndex = i;

                m_TileEditor->SelectTile(TileIndex);
            }
        }

        UIEndFrame();
    }
}
void Dystopia::Render2D()
{
}

void Dystopia::Render3D()
{
}

void Dystopia::UpdateAnimation(Character2DControllerComponent& controller, BitEngine::TransformComponent& transform)
{
    if (controller.MoveInput > 0.0f)
        transform.Scale.x = abs(transform.Scale.x);
    else if (controller.MoveInput < 0.0f)
        transform.Scale.x = -abs(transform.Scale.x); 
    
    if (!controller.IsGrounded)
    {
        m_Animation2DSystem->SetAnimation(player, "wolfIdle");  // jump animation 
    }
    else if (abs(controller.Velocity.x) > 10.0f)
    {
        m_Animation2DSystem->SetAnimation(player, "wolfIdle");
    }
    else
    {
        m_Animation2DSystem->SetAnimation(player, "wolfIdle");
    }
}
void Dystopia::Update(f32 deltaTime)
{

    auto& transform = player.GetComponent<BitEngine::TransformComponent>();
    auto& controller = player.GetComponent<Character2DControllerComponent>();
    auto& rigidbody = player.GetComponent<BitEngine::Rigidbody2DComponent>();
    if(rigidbody.MultiColliderComponents.empty())
    {
        BIT_LOG_ERROR("Player has no colliders!");
        return;
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_L) && !BitEngine::InputWasKeyDown(BitEngine::KEY_L))
    {
        TileIndex++;
        m_TileEditor->SelectTile(TileIndex);
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_K) && !BitEngine::InputWasKeyDown(BitEngine::KEY_K))
    {
        if(TileIndex > 0)
            TileIndex--;

        m_TileEditor->SelectTile(TileIndex);
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_O) && !BitEngine::InputWasKeyDown(BitEngine::KEY_O))
    {
        printf("Enter The name of the map to save: \n");
        char filename[30];
        scanf("%s", filename);
        m_TileEditor->SaveTileMap(filename); 
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_P) && !BitEngine::InputWasKeyDown(BitEngine::KEY_P))
    {
        printf("Enter The name of the map to load:\n");
        char filename[30];
        scanf("%s", filename);
        m_TileEditor->LoadTileMap(filename);
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_Q) && !BitEngine::InputWasKeyDown(BitEngine::KEY_Q))
    {
        drawUI = !drawUI;
    }

    auto& boxCollider = rigidbody.MultiColliderComponents[0].BoxCollider2D;
    BMath::Vec2 currentPos = BitEngine::Physics2DGetPosition(rigidbody.BodyId);
    
    playerController.HandleInput(controller, deltaTime);
    playerController.HandleMovement(controller, deltaTime);
    playerController.HandleJump(controller, deltaTime);
    playerController.HandleGravity(controller, deltaTime);

    BMath::Vec2 finalPos = playerController.ResolveTileCollisionSweep(deltaTime, currentPos, boxCollider, m_TileEditor, controller);

    
    BitEngine::Physics2DSetPosition(rigidbody.BodyId, finalPos);
    rigidbody.Position = BMath::Vec3(finalPos.x, finalPos.y, rigidbody.Position.z);
    transform.Position.x = finalPos.x;
    transform.Position.y = finalPos.y;
    

    BMath::Vec3 cameraPos = transform.Position;
    cameraPos.z = 0;
    ActiveWorldCamera->SetPosition(cameraPos);


}

