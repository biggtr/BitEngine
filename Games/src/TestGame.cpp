#include "TestGame.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/InputSystem.h"
#include "Bit/Font/Font.h"

void TestGame::Initialize()
{
    BPhysics2D::Initialize();
    Assets().AddTexture("Player", "assets/textures/player.png");
    Assets().AddTexture("Tree", "assets/textures/tree.png");
    Assets().AddTexture("background", "assets/textures/blazes/snow/snow_bg.png");
    auto entity = Entities().CreateEntity();
    entity.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -10.0f),
            BMath::Vec3(1000.0f, 1000.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    auto tree = Entities().CreateEntity();
    tree.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -7.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f));
    auto& treesprite = tree.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Tree")
            );
    treesprite.IsUI = false;
    auto& sprite = entity.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("background")
            );
    m_Player = Entities().CreateEntity();
    m_Player.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -5.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    m_Player.AddComponent<BitEngine::CInput>();
    auto& playersprite = m_Player.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player"),
            32, 32
            );
    playersprite.IsUI = false;
    sprite.IsUI = false;
    auto circle1 = Entities().CreateEntity();
    circle1.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(400, 300, -7), 
            BMath::Vec3(200, 200, 0),
            (0.0f)
            );
    auto& c1 = circle1.AddComponent<BitEngine::CCircle>();
    c1.Color = {1.0f, 0.0f, 0.0f, 1.0f}; 

    auto circle2 = Entities().CreateEntity();
    circle2.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(450, 350, -7), 
            BMath::Vec3(200, 200, 0),
            (0.0f)
            );
    auto& c2 = circle2.AddComponent<BitEngine::CCircle>();
    c2.Color = {0.0f, 0.0f, 1.0f, 1.0f}; 

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
    if(Inputs().IsMouseButtonDown(BitEngine::MOUSE_BUTTON_LEFT))
    {
        BMath::Vec3 cameraPos = Camera().GetActiveCamera()->Position;
        i32 screenX, screenY;
        Inputs().GetMousePosition(&screenX, &screenY);
        f32 normalizedX = (f32)screenX / (f32)appConfig.width;
        f32 normalizedY = (f32)(appConfig.height - screenY) / (f32)appConfig.height; // Flip Y
        BIT_LOG_DEBUG("Screen: (%d, %d), Camera: (%.2f, %.2f), WorldDims: (%.2f, %.2f)", 
                  screenX, screenY, cameraPos.x, cameraPos.y, m_WorldWidth, m_WorldHeight);
        f32 worldX = cameraPos.x + normalizedX * m_WorldWidth; 
        f32 worldY = cameraPos.y + normalizedY * m_WorldHeight; 
        BIT_LOG_DEBUG("Screen: (%d, %d) -> Normalized: (%.2f, %.2f) -> World: (%.2f, %.2f)", 
                  screenX, screenY, normalizedX, normalizedY, worldX, worldY);
        BMath::Vec3 mousePos = {
            worldX,
            worldY,
            -7.0f
        };

        u32 circle = BPhysics2D::BCreateCircleShape(12.0f);
        u32 body = BPhysics2D::CreateBody(circle, mousePos, 0.0f);
        m_Bodies.push_back(body);
        auto circleEntity= Entities().CreateEntity();
        circleEntity.AddComponent<BitEngine::CCircle>();
        circleEntity.AddComponent<BitEngine::CTransform>(
                mousePos,
                BMath::Vec3(300.0f, 300.0f, 0.0f),
                (0.0f)
                );
    }

    BMath::Vec3 cameraPos = BMath::Vec3(playerPosition.x - (m_WorldWidth / 2.0f), playerPosition.y - (m_WorldHeight / 2.0f), 0.0f);
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
