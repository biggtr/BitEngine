#include "TestGame.h"
#include "Bit/Math/Vector.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/Animation2DSystem.h"

void TestGame::Initialize()
{
    Assets().AddTexture("Player", "assets/textures/player.png");
    m_Player = Entities().CreateEntity();
    m_Player.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(600.0f, 400.0f, 0.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    auto entity = Entities().CreateEntity();
    entity.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player"),
            32, 32, 1
            );
    entity.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(400.0f, 400.0f, 0.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );

    auto& playersprite = m_Player.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player") 
            );

    playersprite.FrameHeight = playersprite.FrameWidth = 32;
    m_Player.AddComponent<BitEngine::CAnimation2DController>();
    u8 frameCount = 4; 
    m_Animation2DSystem->CreateAnimation(m_Player, "runRight", frameCount, 0, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runLeft", frameCount, 4, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runTop", frameCount, 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runBottom", frameCount, 12, 0.1f);


}

void TestGame::Render()
{

}
void TestGame::Update(f32 deltaTime)
{
    f32 movementSpeed = 1150.0f * deltaTime;
    auto& playerTransform = m_Player.GetComponent<BitEngine::CTransform>().Position;
    if(Inputs().IsKeyDown(BitEngine::KEY_A))
    {
        playerTransform.x -= movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runLeft");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_D))
    {
        playerTransform.x += movementSpeed; 
        m_Animation2DSystem->SetAnimation(m_Player, "runRight");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_W))
    {
        playerTransform.y += movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runTop");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_S))
    {
        playerTransform.y -= movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runBottom");
    }
    BMath::Vec3 cameraPos = BMath::Vec3(playerTransform.x - (1920.0f / 2.0f), playerTransform.y - (1080.0f / 2.0f), 0.0f);
    Camera().SetPosition(cameraPos);
}
