#include "Dystopia.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Font/Font.h"
#include "Bit/Math/BMath.h"
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
#include "box2d/box2d.h"
#include <cstdio>
#include <cstring>

void Dystopia::Initialize()
{

    m_EnemyManager = new EnemyManager(m_ECS);
    m_PlayerController.SetECS(m_ECS);
    // BitEngine::PlatformHideCursor();
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::ORTHO);

    BitEngine::Texture* charactersSprite = BitEngine::AssetStoreAddTexture("CharactersSprite", "assets/textures/CharactersSprite.png");

    TileIndex = 0;
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();
    player.AddComponent<BitEngine::SpriteComponent>();
    player.AddComponent<BitEngine::Character2DControllerComponent>();
    player.AddComponent<BitEngine::Animation2DControllerComponent>();

    weapon = m_ECS->CreateEntity();
    weapon.AddComponent<BitEngine::TransformComponent>();
    weapon.AddComponent<BitEngine::SpriteComponent>();
    weapon.AddComponent<BitEngine::Rigidbody2DComponent>();

    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    playerSprite.STexture = charactersSprite;
    playerSprite.Color = BMath::Vec4(1,1,1,1);
    playerSprite.CurrentFrame = 0;
    playerSprite.FrameWidth = 16;
    playerSprite.FrameHeight = 16;
    playerSprite.Width = 16 * 8;
    playerSprite.Height = 16 * 8;
    m_SpawnLocation = BMath::Vec3(0.0f, 100.0f, -5.0f);
    f32 width = 10;
    f32 height = 10;
    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = m_SpawnLocation;
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {width, height, 1.0f};

    m_Animation2DSystem->CreateAnimation(player, "PlayerJump", 4, 5 * 8, 0.4f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerRun", 4, 6 * 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerIdle", 4, 7 * 8, 0.1f);

    auto& playerRigidBody = player.AddComponent<BitEngine::Rigidbody2DComponent>();
    playerRigidBody.Position = {0.0f, 100.0f, 0.0f};
    playerRigidBody.Type  = BitEngine::PhysicsBodyType::Kinematic;
    m_Physics2DSystem->CreateBoxShape(player, width, height, {0,0}, 0.0f, true, true, PhysicsCategories::PLAYER, PhysicsCategories::ENEMY);

    m_Physics2DSystem->CreateBoxShape(weapon, 10, 5, {0,0}, 30, true, true, PhysicsCategories::WEAPON, PhysicsCategories::ENEMY);

    auto& weaponTransform = weapon.GetComponent<BitEngine::TransformComponent>();
    weaponTransform.Position = playerTransform.Position + BMath::Vec3(5,15,0);
    weaponTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    weaponTransform.Scale = {10, 10, 1.0f};

    auto& weaponSprite = weapon.GetComponent<BitEngine::SpriteComponent>();
    weaponSprite.STexture = charactersSprite;
    weaponSprite.Color = BMath::Vec4(1,1,1,1);
    weaponSprite.CurrentFrame = 8 * 5 + 5;
    weaponSprite.FrameWidth = 16;
    weaponSprite.FrameHeight = 16;
    weaponSprite.Width = 16 * 8;
    weaponSprite.Height = 16 * 8;
    
    auto& controller = player.GetComponent<BitEngine::Character2DControllerComponent>();
    controller.WeaponFocusPosition = weaponTransform.Position;
    controller.WeaponFocusPoint = controller.WeaponFocusPosition;


    m_TileEditor->CreateTileSet(charactersSprite, 16 * 8, 16 * 8, 16, 16);
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

void Dystopia::SpawnEnemy(f32 deltaTime)
{
    if(m_EnemySpawnCoolDown <= 0.0f)
    {
        m_EnemyManager->AddEnemy(ENEMY_TYPE::CAT, m_EnemySpawnLocation, player);
        m_EnemySpawnCoolDown = m_EnemySpawnCoolDownMax;
    }

    m_EnemySpawnCoolDown -= deltaTime;
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
    auto& sprite = player.GetComponent<BitEngine::SpriteComponent>();
    auto& weaponRigidbody = weapon.GetComponent<BitEngine::Rigidbody2DComponent>();
    controller.WeaponFocusPosition = transform.Position + BMath::Vec3(8,2,0);


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

    
    BitEngine::Physics2DSetPosition(rigidbody.BodyId, finalPos);
    rigidbody.Position = BMath::Vec3(finalPos.x, finalPos.y, rigidbody.Position.z);
    transform.Position.x = finalPos.x;
    transform.Position.y = finalPos.y;

    BMath::Vec3 cameraPos = transform.Position + BMath::Vec3(5,10,0);
    ActiveWorldCamera->SetPosition(cameraPos);

    m_PlayerController.UpdateWeaponFocusPoint(controller, deltaTime);
    auto& weaponTransform = weapon.GetComponent<BitEngine::TransformComponent>();
    weaponTransform.Position = controller.WeaponFocusPoint;
    BitEngine::Physics2DSetPosition(weaponRigidbody.BodyId, BMath::Vec2(weaponTransform.Position.x, weaponTransform.Position.y));

    i32 sensorCapacity = b2Shape_GetSensorCapacity(weaponRigidbody.PrimaryShapeId);
    std::vector<b2ShapeId> overlaps;
    overlaps.resize(sensorCapacity);

    i32 count = b2Shape_GetSensorData(weaponRigidbody.PrimaryShapeId, overlaps.data(), sensorCapacity);
    overlaps.resize(count);
    for(i32 i = 0; i < count; ++i)
    {
        b2ShapeId visitorID = overlaps[i];
        if(!b2Shape_IsValid(visitorID))
        {
            continue;
        }
        BitEngine::Entity enemyid = m_EnemyManager->GetEnemyByShapeID(visitorID);
        auto* enemy = &m_ECS->GetComponent<Enemy>(enemyid) ;
        m_PlayerController.HandleAttack(controller, enemy, deltaTime);
        BIT_LOG_DEBUG("enemy health %.2f enemyid: %d", enemy->Health, enemy->ID);
    }
    

    b8 wasDead = controller.IsDead;
    controller.IsDead = controller.Health <= 0.0;
    if(!wasDead && controller.IsDead)
    {
        for(u32 i = 0; i < 80; ++i)
        {
            SpawnParticles(transform.Position);
        }
        controller.IsDead = false;
        
        sprite.Color = BMath::Vec4(1,1,1,0);
        controller.Health = controller.MaxHealth;
        
        BitEngine::Physics2DSetPosition(rigidbody.BodyId, {m_SpawnLocation.x, m_SpawnLocation.y});
        transform.Position = m_SpawnLocation;
        sprite.Color = BMath::Vec4(1,1,1,1);
    }
    

    SpawnEnemy(deltaTime);
    
}

void Dystopia::SpawnParticles(BMath::Vec3 position)
{
    ParticleSettings particleSettings;
    particleSettings.StartColor = {1.0, 0.0,0.0,1.0};
    particleSettings.EndColor = {0.4, 0.0, 0.0, 1.0};
    particleSettings.LifeTime = 0.3;
    particleSettings.Position = position;
    Particle2DSystemEmit(particleSettings);
}
