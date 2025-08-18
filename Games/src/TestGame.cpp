#include "TestGame.h"
#include "Bit/Core/Input.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/InputSystem.h"

void TestGame::Initialize()
{
    Assets().AddTexture("Player", "assets/textures/player.png");
    Assets().AddTexture("Tree", "assets/textures/tree.png");
    Assets().AddTexture("background", "assets/textures/blazes/snow/snow_bg.png");

    m_Background = Entities().CreateEntity();
    m_Background.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("background")
            );
    m_Background.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(1000.0f, 1000.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    m_Player = Entities().CreateEntity();
    m_Player.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(600.0f, 400.0f, 0.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );

    m_Player.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player"),
            32, 32
            );

    m_Player.AddComponent<BitEngine::CAnimation2DController>();
    u8 frameCount = 4; 
    m_Animation2DSystem->CreateAnimation(m_Player, "runRight", frameCount, 0, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runLeft", frameCount, 4, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runTop", frameCount, 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runBottom", frameCount, 12, 0.1f);


}

void TestGame::Render()
{
    BitEngine::CSprite& sprite = m_Background.GetComponent<BitEngine::CSprite>();
    Renderer().DrawQuad(
            BMath::Vec3(100.0f, 100.0f, -4.0f),
            BMath::Vec3(1000.0f, 1000.0f, 0.0f),
            sprite
            );

}
void TestGame::Update(f32 deltaTime)
{
    f32 movementSpeed = 1150.0f * deltaTime;
    auto& playerPosition = m_Player.GetComponent<BitEngine::CTransform>().Position;
    if(Inputs().IsKeyDown(BitEngine::KEY_A))
    {
        playerPosition.x -= movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runLeft");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_D))
    {
        playerPosition.x += movementSpeed; 
        m_Animation2DSystem->SetAnimation(m_Player, "runRight");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_W))
    {
        playerPosition.y += movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runTop");
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_S))
    {
        playerPosition.y -= movementSpeed;
        m_Animation2DSystem->SetAnimation(m_Player, "runBottom");
    }
    BMath::Vec3 cameraPos = BMath::Vec3(playerPosition.x - (1920.0f / 2.0f), playerPosition.y - (1080.0f / 2.0f), 0.0f);
    Camera().SetPosition(cameraPos);
}
void TestGame::OnJump(const BitEngine::Entity& entity)
{

}

void TestGame::SetupInput()
{
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_FORWARD, m_Player, BitEngine::KEY_W, 1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_BACKWARD, m_Player, BitEngine::KEY_S, -1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_LEFT, m_Player, BitEngine::KEY_D, 1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_RIGHT, m_Player, BitEngine::KEY_A, -1.0f);

    m_InputSystem->CreateAction(BitEngine::ACTION_JUMP, m_Player, BitEngine::KEY_SPACE);
    m_InputSystem->CreateAction(BitEngine::ACTION_ATTACK, m_Player, BitEngine::KEY_X); 
    
    m_InputSystem->BindAction(BitEngine::ACTION_JUMP, m_Player, this, &TestGame::OnJump);
}
