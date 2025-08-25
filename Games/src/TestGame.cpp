#include "TestGame.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/InputSystem.h"
#include "Bit/Font/Font.h"

void TestGame::Initialize()
{
    Assets().AddTexture("Player", "assets/textures/player.png");
    Assets().AddTexture("Tree", "assets/textures/tree.png");
    Assets().AddTexture("background", "assets/textures/blazes/snow/snow_bg.png");

    for(u32 i = 0; i < 1000; ++i)
    {
        auto entity = Entities().CreateEntity();
        f32 x = (i % 50) * 80.0f;
        f32 y = ((f32)i / 50) * 80.0f;
        entity.AddComponent<BitEngine::CTransform>(
                BMath::Vec3(x, y, -3.0f),
                BMath::Vec3(100.0f, 100.0f, 0.0f),
                BMath::Vec3(0.0f, 0.0f, 0.0f)
                );
        entity.AddComponent<BitEngine::CSprite>(
                Assets().GetTexture("Tree")
                );
    }
    
    m_Background = Entities().CreateEntity();
    auto& backgroundsprite = m_Background.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("background")
            );
    backgroundsprite.IsUI = true;
    m_Background.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -4.0f),
            BMath::Vec3(1000.0f, 1000.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    m_Player = Entities().CreateEntity();
    m_Player.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, 0.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    m_Player.AddComponent<BitEngine::CInput>();
    auto& playersprite = m_Player.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player"),
            32, 32
            );
    playersprite.IsUI = false;

    m_Player.AddComponent<BitEngine::CAnimation2DController>();
    u8 frameCount = 4; 
    m_Animation2DSystem->CreateAnimation(m_Player, "runRight", frameCount, 0, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runLeft", frameCount, 4, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runTop", frameCount, 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runBottom", frameCount, 12, 0.1f);


    //experimenting with fonts

    BitEngine::Font::ParseFont("assets/fonts/dejavu.ttf");
    BitEngine::Font::Shutdown();

}

void TestGame::Render()
{
    Renderer().DrawCircle(
            BMath::Vec3(300.0f, 300.0f, -3.0f), 
            BMath::Vec3(300.0f, 300.0f, 0.0f),
            BMath::Vec4(1.0f,0.0f, 1.0f, 1.0f), 1.0f);
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
    BIT_LOG_DEBUG("Player JUMP..!");
}

void TestGame::SetupInput()
{
    BIT_LOG_DEBUG("CREATING ACTIONS AND INPUTS");
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_FORWARD, m_Player, BitEngine::KEY_W, 1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_BACKWARD, m_Player, BitEngine::KEY_S, -1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_LEFT, m_Player, BitEngine::KEY_D, 1.0f);
    m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_RIGHT, m_Player, BitEngine::KEY_A, -1.0f);

    m_InputSystem->CreateAction(BitEngine::ACTION_JUMP, m_Player, BitEngine::KEY_SPACE);
    m_InputSystem->CreateAction(BitEngine::ACTION_ATTACK, m_Player, BitEngine::KEY_X); 
    
    m_InputSystem->BindAction(BitEngine::ACTION_JUMP, m_Player, this, &TestGame::OnJump);
}
