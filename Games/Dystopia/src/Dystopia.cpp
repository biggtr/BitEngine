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
    ActiveWorldCamera->SetPosition(BMath::Vec3(0.0f, 0.0f, 10.0f)); 
    BitEngine::Entity player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();

    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {32.0f, 32.0f, 1.0f};
        BIT_LOG_DEBUG("Scale in Initialize(): %.2f, %.2f, %.2f", 
            playerTransform.Scale.x, 
            playerTransform.Scale.y, 
            playerTransform.Scale.z);
    player.AddComponent<BitEngine::SpriteComponent>();
    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    
    m_AssetManager->AddTexture("tree", "assets/textures/tree.png");
    playerSprite.STexture = m_AssetManager->GetTexture("tree");
    playerSprite.FrameWidth = 360;
    playerSprite.FrameHeight = 303;
    playerSprite.CurrentFrame = 1;
    playerSprite.Width = playerSprite.STexture->GetWidth();
    playerSprite.Height = playerSprite.STexture->GetWidth();
    playerSprite.IsUI = false;
    // player.AddComponent<BitEngine::Animation2DControllerComponent>();
    //
    // m_Animation2DSystem->CreateAnimation(player, "playerIdle", 8, 1, 0.4f);

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
}

