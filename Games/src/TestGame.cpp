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

    BMath::Mat4 transform = BMath::Mat4::CreateTransform({appConfig.width/2.0f, appConfig.height / 2.0f, -3.0f}, {200.0f, 200.0f, 0.0f});
    Renderer().DrawCircle(transform , {0.0f, 1.0f, 0.0f, 1.0f}, 1.0f);
    for(auto& body : m_Bodies)
    {
        BPhysics2D::BBody physicsBody = BPhysics2D::GetBody(body);
        BMath::Vec3 renderPos = physicsBody.Position;
        renderPos.z = -3.0f;
        BIT_LOG_DEBUG("Pos from render : %f, %f, %f", physicsBody.Position.x, physicsBody.Position.y, physicsBody.Position.z);
        BMath::Mat4 transform = BMath::Mat4::CreateTransform(renderPos, {200.0f, 200.0f, 0.0f});
        Renderer().DrawCircle(transform , {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f);
    }
    // BPhysics2D::BShape physicsShape = BPhysics2D::GetShape(&physicsBody);
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
        i32 x,y;
        Inputs().GetMousePosition(&x, &y);
        // f32 screenX = (f32)x;
        // f32 screenY = appConfig.height - (f32)y;
        BMath::Vec3 mousePos = {
            (f32)x,
            (f32)y,
            -4.0f
        };

        u32 circle = BPhysics2D::BCreateCircleShape(12.0f);
        u32 body = BPhysics2D::CreateBody(circle, mousePos, 0.0f);
        m_Bodies.push_back(body);
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
