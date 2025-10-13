#include "PoolGame.h"
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

#define BALL_RADIUS 2.0f
#define BALL_MASS 40.0f
#define BALL_GAP 2.0f

void RespawnWhiteBall(BitEngine::Entity whiteBall)
{
    auto& transform = whiteBall.GetComponent<BitEngine::TransformComponent>();
    transform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);

    auto& rigid = whiteBall.GetComponent<BitEngine::Rigid2DBodyComponent>();
    BitEngine::BBody& body = BitEngine::BPhysics2DGetBody(rigid.BodyIndex);
    body.Position = BMath::Vec3( 0.0f, 0.0f, -5.0f);
    body.Velocity = BMath::Vec3( 0.0f, 0.0f, 0.0f );
}
BitEngine::Entity PoolGame::CreateTable(BitEngine::Entity* outPockets, f32 width, f32 height)
{
    auto table = m_ECS->CreateEntity();
    table.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(width, height, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    table.AddComponent<BitEngine::SpriteComponent>().Color = {0.0f, 0.4f, 0.0f, 1.0f};

    auto uptableEdge = m_ECS->CreateEntity();
    uptableEdge.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, height / 2.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
        uptableEdge.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 1.0f, 1.0f, 1.0f};
    uptableEdge.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
    uptableEdge.AddComponent<BitEngine::Box2DColliderComponent>(BMath::Vec3(width - 10.0f , 5.0f, 0.0f));

    auto downtableEdge = m_ECS->CreateEntity();
    downtableEdge.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, -height / 2.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
        downtableEdge.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 1.0f, 1.0f, 1.0f};
    downtableEdge.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
    downtableEdge.AddComponent<BitEngine::Box2DColliderComponent>(BMath::Vec3(width - 10.0f, 5.0f, 0.0f));

    auto lefttableEdge = m_ECS->CreateEntity();
    lefttableEdge.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(-width/ 2.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
        lefttableEdge.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 1.0f, 1.0f, 1.0f};
    lefttableEdge.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
    lefttableEdge.AddComponent<BitEngine::Box2DColliderComponent>(BMath::Vec3(5.0f, height - 10.0f, 0.0f));

    auto righttableEdge = m_ECS->CreateEntity();
    righttableEdge.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(width/ 2.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
        righttableEdge.AddComponent<BitEngine::SpriteComponent>().Color = {1.0f, 1.0f, 1.0f, 1.0f};
    righttableEdge.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
    righttableEdge.AddComponent<BitEngine::Box2DColliderComponent>(BMath::Vec3(5.0f, height - 10.0f, 0.0f));

    // Create Table Pockets
    for(i32 i = 0; i < 6; ++i)
    {
        f32 x = (i % 3 - 1) * (width / 2.0f);
        f32 y = (i < 3 ? 1 : -1) * (height / 2.0f);

        BMath::Vec3 pocketPos = {x, y, -5.0f};

        auto pocket = m_ECS->CreateEntity();
        pocket.AddComponent<BitEngine::TransformComponent>(pocketPos);
        pocket.AddComponent<BitEngine::Circle2DComponent>(BALL_RADIUS * 3);
        pocket.AddComponent<BitEngine::Circle2DColliderComponent>(BALL_RADIUS * 3);
        pocket.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f);
        outPockets[i] = pocket;
    }

    return table;
}
void PoolGame::CreateBalls(BitEngine::Entity* outEntities, u8 row)
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
        auto ball = m_ECS->CreateEntity();
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

    m_WhiteBall = m_ECS->CreateEntity();
    m_WhiteBall.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f),
            BMath::Vec3(1.0f)
            );


    m_WhiteBall.AddComponent<BitEngine::Circle2DComponent>(BALL_RADIUS);
    m_WhiteBall.AddComponent<BitEngine::Circle2DColliderComponent>(BALL_RADIUS);
    m_WhiteBall.AddComponent<BitEngine::Rigid2DBodyComponent>(BALL_MASS);
    outEntities[i] = m_WhiteBall;
    m_ActiveBallCount = totalNumBalls + 1;  
    BIT_LOG_INFO("Created %u balls total (%u colored + 1 white)", m_ActiveBallCount, totalNumBalls);
}
void PoolGame::Initialize()
{
    CreateBalls(m_Balls, 5);

    m_Table = CreateTable(m_Pockets, 150.0f, 65.0f);

    isDragging = false;
}
void PoolGame::UIRender()
{
    // BitUI::BeginLayout(BitUI::HorizontalLayout(100.0f, 100.0f, 5.0f));
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // std::vector<BitUI::DrawCommand> horizontalDrawCommands =  BitUI::EndLayout();
    //
    // BitUI::BeginLayout(BitUI::VerticalLayout(100.0f, 300.0f, 5.0f));
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // BitUI::Button("blabla", 100.0f, 100.0f);
    // std::vector<BitUI::DrawCommand> verticalDrawCommands =  BitUI::EndLayout();
    // for(auto& command : horizontalDrawCommands)
    // {
    //     m_Renderer2D->.DrawQuad(command.Position, command.Size, command.data.Color);
    // }
    // for(auto& command : verticalDrawCommands)
    // {
    //     m_Renderer2D->.DrawQuad(command.Position, command.Size, command.data.Color);
    // }
}
void PoolGame::Render()
{
    if(isDragging)
    {
        BIT_LOG_DEBUG("its dragging");
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
        m_Renderer2D->DrawLine(startPos, startPos + dragDirNorm * lineLength, {1.0f,0.0f, 0.0f, 1.0f});
    }
}
void PoolGame::Update(f32 deltaTime)
{
    for (int j = m_ActiveBallCount - 1; j >= 0; --j)
    {
        BitEngine::Entity ball = m_Balls[j];
        b8 ballPocketed = false;

        for (u32 i = 0; i < 6; ++i)
        {
            BitEngine::Entity pocket = m_Pockets[i];

            if (m_ECS->HasComponent<BitEngine::Rigid2DBodyComponent>(ball))
            {
                BitEngine::Rigid2DBodyComponent& rigidBodyA = m_ECS->GetComponent<BitEngine::Rigid2DBodyComponent>(ball);
                BitEngine::Rigid2DBodyComponent& rigidBodyB = m_ECS->GetComponent<BitEngine::Rigid2DBodyComponent>(pocket);
                BitEngine::BBody& ballBody = BitEngine::BPhysics2DGetBody(rigidBodyA.BodyIndex);
                BitEngine::BBody& pocketBody = BitEngine::BPhysics2DGetBody(rigidBodyB.BodyIndex);

                BitEngine::BPhysics2DContact contact;
                if (BitEngine::BPhysics2DIsColliding(&ballBody, &pocketBody, contact))
                {
                    if (ball == m_WhiteBall)
                    {
                        RespawnWhiteBall(m_WhiteBall);
                    }
                    else
                    {
                        ball.KillEntity();

                        m_Balls[j] = m_Balls[m_ActiveBallCount - 1];

                        m_ActiveBallCount--;
                    }
                    
                    ballPocketed = true;
                    break; 
                }
            }
        }
    }
    (void)deltaTime;

    if(BitEngine::InputIsMouseButtonPressed(BitEngine::MOUSE_BUTTON_RIGHT)) 
    {
        BitEngine::InputGetMousePosition(&initialMouseX, &initialMouseY);

        isDragging = true;
    }
    if(isDragging && BitEngine::InputIsMouseButtonDown(BitEngine::MOUSE_BUTTON_RIGHT))
    {
        if(BitEngine::InputIsMouseButtonDown(BitEngine::MOUSE_BUTTON_RIGHT))
        {
            BitEngine::InputGetMousePosition(&currentMouseX, &currentMouseY);
        }
    }
    if(isDragging && BitEngine::InputIsMouseButtonReleased(BitEngine::MOUSE_BUTTON_RIGHT))
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

// void TestGame::SetupBitEngine::Input()
// {
    // BIT_LOG_DEBUG("CREATING ACTIONS AND INPUTS");
    // m_BitEngine::InputSystem->CreateAxis(BitEngine::ACTION_MOVE_FORWARD, m_Player, BitEngine::InputKEY_W, 1.0f);
    // m_BitEngine::InputSystem->CreateAxis(BitEngine::ACTION_MOVE_BACKWARD, m_Player, BitEngine::InputKEY_S, -1.0f);
    // m_BitEngine::InputSystem->CreateAxis(BitEngine::ACTION_MOVE_LEFT, m_Player, BitEngine::InputKEY_D, 1.0f);
    // m_BitEngine::InputSystem->CreateAxis(BitEngine::ACTION_MOVE_RIGHT, m_Player, BitEngine::InputKEY_A, -1.0f);
    //
    // m_BitEngine::InputSystem->CreateAction(BitEngine::ACTION_JUMP, m_Player, BitEngine::InputKEY_SPACE);
    // m_BitEngine::InputSystem->CreateAction(BitEngine::ACTION_ATTACK, m_Player, BitEngine::InputKEY_X); 
    
    // m_BitEngine::InputSystem->BindAction(BitEngine::ACTION_JUMP, m_Player, this, &TestGame::OnJump);
