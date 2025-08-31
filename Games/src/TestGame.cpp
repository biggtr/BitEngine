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
    Assets().AddTexture("Player", "assets/textures/player.png");
    Assets().AddTexture("Tree", "assets/textures/tree.png");
    Assets().AddTexture("background", "assets/textures/blazes/snow/snow_bg.png");
    auto entity = Entities().CreateEntity();
    entity.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -10.0f),
            BMath::Vec3(1000.0f, 1000.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    auto tree = Entities().CreateEntity();
    tree.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -7.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f));
    tree.AddComponent<BitEngine::SpriteComponent>(
            Assets().GetTexture("Tree")
            );
    auto& treesprite = tree.GetComponent<BitEngine::SpriteComponent>();
    treesprite.IsUI = false;
    entity.AddComponent<BitEngine::SpriteComponent>(
            Assets().GetTexture("background")
            );
    auto& sprite = entity.GetComponent<BitEngine::SpriteComponent>();
    sprite.IsUI = false;
    m_Player = Entities().CreateEntity();
    m_Player.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -5.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    m_Player.AddComponent<BitEngine::CInput>();
    m_Player.AddComponent<BitEngine::SpriteComponent>(
            Assets().GetTexture("Player"),
            32, 32
            );
    auto& playersprite = m_Player.GetComponent<BitEngine::SpriteComponent>();
    playersprite.IsUI = false;

    m_Player.AddComponent<BitEngine::Animation2DControllerComponent>();
    u8 frameCount = 4; 
    m_Animation2DSystem->CreateAnimation(m_Player, "runRight", frameCount, 0, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runLeft", frameCount, 4, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runTop", frameCount, 8, 0.1f);
    m_Animation2DSystem->CreateAnimation(m_Player, "runBottom", frameCount, 12, 0.1f);

    isDragging = false;
    auto whiteBall = Entities().CreateEntity();
    whiteBall.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(appConfig.width / 2.0f, appConfig.height / 2.0f, -5.0f),
            BMath::Vec3(0.0f),
            BMath::Vec3(1.0f)
            );
    whiteBall.AddComponent<BitEngine::Circle2DComponent>(300.0f);
    whiteBall.AddComponent<BitEngine::Circle2DColliderComponent>(300.0f);
    whiteBall.AddComponent<BitEngine::Rigid2DBodyComponent>(12.0f);

    //experimenting with fonts

    BitEngine::Font::ParseFont("assets/fonts/dejavu.ttf");
    BitEngine::Font::Shutdown();

}
void TestGame::Render()
{
    if(isDragging)
    {
        BMath::Vec3 initialMousePos = ScreenToWorldCoords(initialMouseX, initialMouseY);
        BMath::Vec3 endMousePos = ScreenToWorldCoords(currentMouseX, currentMouseY);
        BIT_LOG_DEBUG("initiamousepos %.2f, %.2f", initialMousePos.x, initialMousePos.y);
        BIT_LOG_DEBUG("endMousePos %.2f, %.2f", endMousePos.x, endMousePos.y);
        Renderer().DrawLine(initialMousePos, endMousePos, {1.0f,0.0f, 0.0f, 1.0f});
    }
}
void TestGame::Update(f32 deltaTime)
{
    if(Inputs().IsMouseButtonPressed(BitEngine::MOUSE_BUTTON_RIGHT)) 
    {
        Inputs().GetMousePosition(&initialMouseX, &initialMouseY);
        isDragging = true;
        BIT_LOG_DEBUG("Mouse RIGHT pressed - started dragging at %d, %d", initialMouseX, initialMouseY);
    }
    if(isDragging && Inputs().IsMouseButtonDown(BitEngine::MOUSE_BUTTON_RIGHT))
    {
        if(Inputs().IsMouseButtonDown(BitEngine::MOUSE_BUTTON_RIGHT))
        {
            Inputs().GetMousePosition(&currentMouseX, &currentMouseY);
        }
    }
    if(isDragging && Inputs().IsMouseButtonReleased(BitEngine::MOUSE_BUTTON_RIGHT))
    {
        BMath::Vec3 impulse = {(f32)(currentMouseX - initialMouseX), (f32)(currentMouseY - initialMouseY), 0.0f};
        isDragging = false;
    }
    if(Inputs().IsMouseButtonDown(BitEngine::MOUSE_BUTTON_LEFT))
    {
        i32 screenX, screenY;
        Inputs().GetMousePosition(&screenX, &screenY);

        BMath::Vec3 mousePos = ScreenToWorldCoords(screenX, screenY);
        mousePos.z = -7.0f;


        auto circleEntity= Entities().CreateEntity();
        circleEntity.AddComponent<BitEngine::TransformComponent>(
                mousePos,
                (0.0f),
                (0.0f)
                );
        auto& circleSprite = circleEntity.AddComponent<BitEngine::Circle2DComponent>(50.0f);
        circleSprite.Color = {1.0f, 0.4f, 0.0f, 1.0f};
        circleEntity.AddComponent<BitEngine::Circle2DColliderComponent>(50.0f);
        circleEntity.AddComponent<BitEngine::Rigid2DBodyComponent>(100.0f);
    }


    f32 movementSpeed = 1150.0f * deltaTime;
    auto& playerPosition = m_Player.GetComponent<BitEngine::TransformComponent>().Position;
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
