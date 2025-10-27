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
    BitEngine::Entity player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    player.AddComponent<BitEngine::SpriteComponent>();

    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    
    m_AssetManager->AddTexture("playersprite", "assets/textures/idle.png");
    playerSprite.STexture = m_AssetManager->GetTexture("playersprite");
    playerSprite.FrameHeight = 32;
    playerSprite.FrameWidth = 32;
    player.AddComponent<BitEngine::Animation2DControllerComponent>();

    m_Animation2DSystem->CreateAnimation(player, "playerIdle", 8, 0, 0.4f);

}
void Dystopia::UIRender()
{
}
void Dystopia::Render()
{
}
void Dystopia::Update(f32 deltaTime)
{
}

