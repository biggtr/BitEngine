#include "Dystopia.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Font/Font.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/System.h"
#include "Bit/Particles/ParticleSystem.h"
#include "Bit/Physics/Physics2D.h"
#include "Bit/Physics/PhysicsTypes.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Resources/AssetStore.h"
#include "Bit/Tiles/Tile.h"
#include "Bit/Tiles/TileLayer.h"
#include "Bit/UI/BitUI.h"
#include "Bit/UI/Widgets.h"
#include "EnemyManager.h"
#include "Platform/Platform.h"
#include "PlayerController.h"
#include <cstdio>
#include <cstring>

void Dystopia::Initialize()
{

    m_EnemyManager = new EnemyManager(m_ECS);
    // BitEngine::PlatformHideCursor();
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::ORTHO);

    BitEngine::Texture* charactersSprite = BitEngine::AssetStoreAddTexture("CharactersSprite", "assets/textures/CharactersSprite.png");

    TileIndex = 0;
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();
    player.AddComponent<BitEngine::SpriteComponent>();
    player.AddComponent<BitEngine::Character2DControllerComponent>();
    player.AddComponent<BitEngine::Animation2DControllerComponent>();

    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    playerSprite.STexture = charactersSprite;
    playerSprite.CurrentFrame = 0;
    playerSprite.FrameWidth = 16;
    playerSprite.FrameHeight = 16;
    playerSprite.Width = 16 * 8;
    playerSprite.Height = 16 * 8;
    
    f32 width = 10;
    f32 height = 10;
    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 100.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {width, height, 1.0f};

    m_Animation2DSystem->CreateAnimation(player, "PlayerJump", 4, 5 * 8, 0.4f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerRun", 4, 6 * 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerIdle", 4, 7 * 8, 0.1f);

    auto& playerRigidBody = player.AddComponent<BitEngine::Rigidbody2DComponent>();
    playerRigidBody.Position = {0.0f, 100.0f, 0.0f};
    playerRigidBody.Type  = BitEngine::PhysicsBodyType::Kinematic;
    m_Physics2DSystem->CreateBoxShape(player, width, height, 0.0f, true, PhysicsCategories::PLAYER, PhysicsCategories::ENEMY);

    


    BitEngine::Texture* tilesetTexture = BitEngine::AssetStoreAddTexture("tileset", "assets/textures/spritesheet.png");
    m_TileEditor->CreateTileSet(charactersSprite, 128, 128, 16, 16);
    m_TileEditor->SelectTile(TileIndex);


    m_TileEditor->CreateTileMap("Level_1", 1000, 1000, 5);
    m_TileEditor->AddLayer("background", BitEngine::TILE_LAYER_TYPE::COLLISION); 
    m_TileEditor->SetActiveLayer(0);

    m_EnemyManager->AddEnemy(ENEMY_TYPE::CAT, BMath::Vec3(10.0f, 100.0f, -5.0f), player);
    m_EnemyManager->AddEnemy(ENEMY_TYPE::CAT, BMath::Vec3(-10.0f, 100.0f, -5.0f), player);
    m_EnemyManager->AddEnemy(ENEMY_TYPE::CAT, BMath::Vec3(20.0f, 100.0f, -5.0f), player);
    m_EnemyManager->AddEnemy(ENEMY_TYPE::CAT, BMath::Vec3(-20.0f, 100.0f, -5.0f), player);

    // FontInitialize("assets/fonts/dejavu.ttf");
}

void Dystopia::RenderUI()
{
    m_TileEditor->DrawUI();
}
void Dystopia::Render2D()
{
}

void Dystopia::Render3D()
{
}

void Dystopia::UpdateAnimation(BitEngine::Character2DControllerComponent& controller, BitEngine::TransformComponent& transform)
{
    if (controller.MoveInput > 0.0f)
        transform.Scale.x = abs(transform.Scale.x);
    else if (controller.MoveInput < 0.0f)
        transform.Scale.x = -abs(transform.Scale.x); 

    if (!controller.IsGrounded)
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerJump");  // jump animation 
    }
    else if (abs(controller.Velocity.x) > 10.0f)
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }
    else
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerIdle");
    }
}
void Dystopia::Update(f32 deltaTime)
{
    auto& transform = player.GetComponent<BitEngine::TransformComponent>();
    auto& controller = player.GetComponent<BitEngine::Character2DControllerComponent>();
    auto& rigidbody = player.GetComponent<BitEngine::Rigidbody2DComponent>();


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


    auto& boxCollider = rigidbody.MultiColliderComponents[0].BoxCollider2D;
    BMath::Vec2 currentPos = BitEngine::Physics2DGetPosition(rigidbody.BodyId);
    
    controller.WasGrounded = controller.IsGrounded;
    m_PlayerController.HandleInput(controller, deltaTime);
    m_PlayerController.HandleMovement(controller, deltaTime);
    m_PlayerController.HandleJump(controller, deltaTime);
    m_PlayerController.HandleGravity(controller, deltaTime);
    UpdateAnimation(controller, transform);
    

    BMath::Vec2 finalPos = m_PlayerController.ResolveTileCollisionSweep(deltaTime, currentPos, boxCollider, m_TileEditor, controller);

    m_EnemyManager->Update(deltaTime, m_TileEditor);
    // if(!controller.WasGrounded && controller.IsGrounded)
    // {
    //     for(u32 i = 0; i < 20; ++i)
    //     {
    //         SpawnParticles(transform.Position + BMath::Vec3(0.0,-5, 0));
    //     }
    // }
    
    BitEngine::Physics2DSetPosition(rigidbody.BodyId, finalPos);
    rigidbody.Position = BMath::Vec3(finalPos.x, finalPos.y, rigidbody.Position.z);
    transform.Position.x = finalPos.x;
    transform.Position.y = finalPos.y;
    
    // b2SensorEvents events = BitEngine::Physics2DGetSensorEvents();
    // for(i32 i = 0; i < events.beginCount; ++i)
    // {
    //     BIT_LOG_DEBUG("contact %d", i);
    // }

    BMath::Vec3 cameraPos = transform.Position;
    cameraPos.x += 5;
    cameraPos.y += 10;
    cameraPos.z = 0;
    ActiveWorldCamera->SetPosition(cameraPos);
    
}

void Dystopia::SpawnParticles(BMath::Vec3 position)
{
    BitEngine::ParticleSettings particleSettings;
    particleSettings.Position = position;
    m_ParticleSystem->Emit(particleSettings);
}
