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
    if(BitEngine::InputIsMouseButtonPressed(BitEngine::MOUSE_BUTTON_LEFT))
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerAttack");
        i32 x;
        i32 y;
        BitEngine::InputGetMousePosition(&x, &y);
        BMath::Vec3 position = ScreenToWorldCoords(x, y);
        position.z = -5;
        BIT_LOG_DEBUG("particle with position from game: %.2f, %.2f, %.2f", position.x, position.y, position.z) 
        for(u32 i = 0; i < 100; ++i)
        {
            m_ParticleSystem->Emit({
                    .Position = position,
                    .Velocity = {5.0f, 4.0f, 0.0f},
                    .VelocityVariation = {2.0f, 1.4f, 0.0f},

                
                    .StartColor = {0.0f, 0.4f, 1.0f, 1.0},
                    .EndColor = {0.0f, 0.4f, 1.0f, 1.0},

                    .SizeVariation = 5.0f,
                    .StartSize = 10.0f,
                    .EndSize = 0.1,

                    .RotationVariation = 4.0f,
                    .StartRotation = 2.0f,
                    .EndRotation = 0.5f,

                    .LifeTimeVariation = 2.0f,
                    .LifeTime = 2.0f,
                    });
        }
    }
    // m_Animation2DSystem->SetAnimation(player, "PlayerDamaged");

}

