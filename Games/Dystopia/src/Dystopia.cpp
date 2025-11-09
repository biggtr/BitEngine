#include "Dystopia.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/System.h"

void Dystopia::Initialize()
{
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();

    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {35.0f, 35.0f, 1.0f};
    player.AddComponent<BitEngine::SpriteComponent>();
    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    
    m_AssetManager->AddTexture("samuri", "assets/textures/samuri.png");
    playerSprite.STexture = m_AssetManager->GetTexture("samuri");
    playerSprite.Width = 21 * 32;
    playerSprite.Height = 64;
    playerSprite.FrameWidth = 32;
    playerSprite.FrameHeight = 32;
    playerSprite.CurrentFrame = 0;
    playerSprite.IsUI = false;

    player.AddComponent<BitEngine::Animation2DControllerComponent>();
    m_Animation2DSystem->CreateAnimation(player, "PlayerRun", 16, 0, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerAttack", 7, 22, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerDamaged", 4, 29, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerIdle", 10, 29 + 4, 0.07f);

    m_AssetManager->AddTexture("dystopiaTileset", "assets/textures/dystopiatiles.png");

    BitEngine::Texture* tilesetTexture = m_AssetManager->AddTexture("tileset", "assets/textures/tileset.png");
    BitEngine::TileSet* tileset = m_TileEditor->CreateTileSet(tilesetTexture, 320.0f, 320.0f, 32, 32);
            

    m_TileEditor->CreateTileMap("Level_1", 32, 32, 32);

}

void Dystopia::RenderUI()
{
}
void Dystopia::Render2D()
{
}

void Dystopia::Render3D()
{
}
void Dystopia::Update(f32 deltaTime)
{
    auto& playertransform = player.GetComponent<BitEngine::TransformComponent>();
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D))
    {
        playertransform.Scale.x *= playertransform.Scale.x < 0 ? -1 : 1; 
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_A))
    {
        playertransform.Scale.x *= playertransform.Scale.x > 0 ? -1 : 1;
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }

}

