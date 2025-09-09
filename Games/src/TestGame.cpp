#include "TestGame.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BCollision.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/InputSystem.h"
#include "Bit/Font/Font.h"
#include "Bit/Systems/System.h"
#include "Bit/UI/BitUI.h"
#include "Bit/UI/Widgets.h"
#include <cmath>

#define BALL_RADIUS 2.0f
#define BALL_MASS 40.0f
#define BALL_GAP 2.0f

void RespawnWhiteBall(BitEngine::Entity whiteBall)
{
    auto& transform = whiteBall.GetComponent<BitEngine::TransformComponent>();
    transform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);

    auto& rigid = whiteBall.GetComponent<BitEngine::Rigid2DBodyComponent>();
    BPhysics2D::BBody& body = BPhysics2D::GetBody(rigid.BodyIndex);
    body.Position = BMath::Vec3( 0.0f, 0.0f, -5.0f);
    body.Velocity = BMath::Vec3( 0.0f, 0.0f, 0.0f );
}
BitEngine::Entity TestGame::CreateTable(BitEngine::Entity* outPockets, f32 width, f32 height)
{
    auto table = Entities().CreateEntity();
    table.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(width, height, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    table.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 0.4f, 0.0f, 1.0f};


    auto tableEdge = Entities().CreateEntity();
    tableEdge.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(10.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
        tableEdge.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 1.0f, 1.0f, 1.0f};
    tableEdge.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
    tableEdge.AddComponent<BitEngine::Box2DColliderComponent>(BMath::Vec3(10.0f, 10.0f, 0.0f));

    // Create Table Pockets
    for(i32 i = 0; i < 6; ++i)
    {
        f32 x = (i % 3 - 1) * (width / 2.0f);
        f32 y = (i < 3 ? 1 : -1) * (height / 2.0f);

        BMath::Vec3 pocketPos = {x, y, -5.0f};

        auto pocket = Entities().CreateEntity();
        pocket.AddComponent<BitEngine::TransformComponent>(pocketPos);
        pocket.AddComponent<BitEngine::Circle2DComponent>(BALL_RADIUS * 3);
        pocket.AddComponent<BitEngine::Circle2DColliderComponent>(BALL_RADIUS * 3);
        pocket.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
        outPockets[i] = pocket;
    }

    return table;
}
void TestGame::CreateBalls(BitEngine::Entity* outEntities, u8 row)
{
    u32 i = 0;
    f32 ballRadius = BALL_RADIUS;
    f32 horizontalGap = 2.2f * ballRadius;
    f32 verticalGap = BALL_GAP;
    u32 totalNumBalls = (row * (row + 1)) / 2;
    BMath::Vec3 startPos = {20.0f, 0.0f, -5.0f}; 
    u32 currentRow = 0;
    u32 ballsInCurrentRow = 0;
    BMath::Vec3 ballPos = startPos;

    while(i < totalNumBalls)
    {
        if(ballsInCurrentRow == currentRow)
        {
            ballsInCurrentRow = 0;
            currentRow++;
            ballPos.x += horizontalGap;
            
            f32 rowWidth = (currentRow - 1) * (ballRadius * 2 + verticalGap);
            ballPos.y = startPos.y - rowWidth * 0.5f;
        }
        else
        {
            ballPos.y += (ballRadius * 2 + verticalGap);
        }
        ballsInCurrentRow++;
        auto ball = Entities().CreateEntity();
        ball.AddComponent<BitEngine::TransformComponent>(
                ballPos,
                0.0f,
                0.0f
                );
        ball.AddComponent<BitEngine::Circle2DComponent>(BALL_RADIUS, BMath::Vec4(1.0f, 0.2f, 0.0f, 1.0f));
        ball.AddComponent<BitEngine::Circle2DColliderComponent>(BALL_RADIUS);
        ball.AddComponent<BitEngine::Rigid2DBodyComponent>(BALL_MASS);
        outEntities[i] = ball;

        i++;
    }

    m_WhiteBall = Entities().CreateEntity();
    m_WhiteBall.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f),
            BMath::Vec3(1.0f)
            );


    m_WhiteBall.AddComponent<BitEngine::Circle2DComponent>(BALL_RADIUS);
    m_WhiteBall.AddComponent<BitEngine::Circle2DColliderComponent>(BALL_RADIUS);
    m_WhiteBall.AddComponent<BitEngine::Rigid2DBodyComponent>(BALL_MASS);
    outEntities[i] = m_WhiteBall;
}
void TestGame::Initialize()
{
    CreateBalls(m_Balls, 5);

    m_Table = CreateTable(m_Pockets, 150.0f, 65.0f);

    isDragging = false;
}
void TestGame::UIRender()
{
    BitUI::BeginLayout(BitUI::HorizontalLayout(100.0f, 100.0f, 5.0f));
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    std::vector<BitUI::DrawCommand> horizontalDrawCommands =  BitUI::EndLayout();

    BitUI::BeginLayout(BitUI::VerticalLayout(100.0f, 300.0f, 5.0f));
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    std::vector<BitUI::DrawCommand> verticalDrawCommands =  BitUI::EndLayout();
    for(auto& command : horizontalDrawCommands)
    {
        Renderer().DrawQuad(command.Position, command.Size, command.data.Color);
    }
    for(auto& command : verticalDrawCommands)
    {
        Renderer().DrawQuad(command.Position, command.Size, command.data.Color);
    }
}
void TestGame::Render()
{
    if(isDragging)
    {
        BMath::Vec3 initialMousePos = ScreenToWorldCoords(initialMouseX, initialMouseY);
        BMath::Vec3 endMousePos = ScreenToWorldCoords(currentMouseX, currentMouseY);
        auto& whiteballTranform = m_WhiteBall.GetComponent<BitEngine::TransformComponent>();
        auto whitePos = whiteballTranform.Position; 
        auto dragDir = endMousePos - initialMousePos;
        BMath::Vec3 dragDirNorm = BMath::Vec3Normalize(dragDir);
        f32 ballRadius = BALL_RADIUS;
        f32 lineBallGap = 1.0f;

        auto startPos = whitePos + dragDirNorm * (ballRadius + lineBallGap);
        f32 lineLength = BMath::Vec3Length(dragDir) * 2.0f;
        Renderer().DrawLine(startPos, startPos + dragDirNorm * lineLength, {1.0f,0.0f, 0.0f, 1.0f});
    }
}
void TestGame::Update(f32 deltaTime)
{
    for(u32 i = 0; i < 6; ++i)
    {
        for(u32 j = 0; j < 16; ++j)
        {
            BitEngine::Entity ball = m_Balls[j];
            BitEngine::Entity pocket = m_Pockets[i];

            if(Entities().HasComponent<BitEngine::Circle2DColliderComponent>(ball) && 
                    Entities().HasComponent<BitEngine::Circle2DColliderComponent>(pocket) &&
                    Entities().HasComponent<BitEngine::Rigid2DBodyComponent>(ball) &&
                    Entities().HasComponent<BitEngine::Rigid2DBodyComponent>(pocket) 
            )
            {
                BitEngine::Rigid2DBodyComponent& rigidBodyA = Entities().GetComponent<BitEngine::Rigid2DBodyComponent>(ball);
                BitEngine::Rigid2DBodyComponent& rigidBodyB = Entities().GetComponent<BitEngine::Rigid2DBodyComponent>(pocket);
                BPhysics2D::BBody& ballBody = BPhysics2D::GetBody(rigidBodyA.BodyIndex);
                BPhysics2D::BBody& pocketBody = BPhysics2D::GetBody(rigidBodyB.BodyIndex);

                BPhysics2D::Contact contact;
                if(BPhysics2D::IsColliding(&ballBody, &pocketBody, contact))
                {
                    if(ball == m_WhiteBall)
                    {
                        BIT_LOG_DEBUG("RespawnWhiteBall");
                        RespawnWhiteBall(m_WhiteBall);
                    }
                    else 
                    {
                        ball.KillEntity();
                    }
                }
        }
    }
    (void)deltaTime;
    if(BitEngine::BInput::IsMouseButtonPressed(BitEngine::BInput::MOUSE_BUTTON_RIGHT)) 
    {
        BitEngine::BInput::GetMousePosition(&initialMouseX, &initialMouseY);

        isDragging = true;
    }
    if(isDragging && BitEngine::BInput::IsMouseButtonDown(BitEngine::BInput::MOUSE_BUTTON_RIGHT))
    {
        if(BitEngine::BInput::IsMouseButtonDown(BitEngine::BInput::MOUSE_BUTTON_RIGHT))
        {
            BitEngine::BInput::GetMousePosition(&currentMouseX, &currentMouseY);
        }
    }
    if(isDragging && BitEngine::BInput::IsMouseButtonReleased(BitEngine::BInput::MOUSE_BUTTON_RIGHT))
    {
        BMath::Vec3 impulse = {(f32)(initialMouseX - currentMouseX), (f32)(currentMouseY - initialMouseY), 0.0f};
        impulse = impulse * 20.0f;
        m_Physics2DSystem->ApplyImpulse(m_WhiteBall, impulse);

        isDragging = false;
    }
}

// void TestGame::OnJump(const BitEngine::Entity& entity)
// {
//     (void)
//     BIT_LOG_DEBUG("Player JUMP..!");
// }

// void TestGame::SetupInput()
// {
    // BIT_LOG_DEBUG("CREATING ACTIONS AND INPUTS");
    // m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_FORWARD, m_Player, BitEngine::BInput::KEY_W, 1.0f);
    // m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_BACKWARD, m_Player, BitEngine::BInput::KEY_S, -1.0f);
    // m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_LEFT, m_Player, BitEngine::BInput::KEY_D, 1.0f);
    // m_InputSystem->CreateAxis(BitEngine::ACTION_MOVE_RIGHT, m_Player, BitEngine::BInput::KEY_A, -1.0f);
    //
    // m_InputSystem->CreateAction(BitEngine::ACTION_JUMP, m_Player, BitEngine::BInput::KEY_SPACE);
    // m_InputSystem->CreateAction(BitEngine::ACTION_ATTACK, m_Player, BitEngine::BInput::KEY_X); 
    
    // m_InputSystem->BindAction(BitEngine::ACTION_JUMP, m_Player, this, &TestGame::OnJump);
}
