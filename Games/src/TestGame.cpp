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
#include "Bit/Systems/System.h"
#include "Bit/UI/BitUI.h"
#include "Bit/UI/Widgets.h"

void TestGame::Initialize()
{


    isDragging = false;
    whiteBall = Entities().CreateEntity();
    whiteBall.AddComponent<BitEngine::TransformComponent>(
            BMath::Vec3(0.0f, 0.0f, -5.0f),
            BMath::Vec3(0.0f),
            BMath::Vec3(1.0f)
            );

    whiteBall.AddComponent<BitEngine::Circle2DComponent>(4.0f);
    whiteBall.AddComponent<BitEngine::Circle2DColliderComponent>(4.0f);
    whiteBall.AddComponent<BitEngine::Rigid2DBodyComponent>(25.0f);

}
void TestGame::UIRender()
{
    BitUI::BeginLayout(BitUI::HorizontalLayout(10.0f, 10.0f, 5.0f));
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    BitUI::Button("blabla", 100.0f, 100.0f);
    std::vector<BitUI::DrawCommand> drawCommands =  BitUI::EndLayout();
    for(auto& command : drawCommands)
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
        auto& whiteballTranform = whiteBall.GetComponent<BitEngine::TransformComponent>();
        auto whitePos = whiteballTranform.Position; 
        auto dragDir = endMousePos - initialMousePos;
        f32 ballRadius = 4.0f;
        f32 lineBallGap = 1.0f;
        auto startPos = whitePos + BMath::Vec3Normalize(dragDir) * (ballRadius + lineBallGap);
        f32 lineLength = BMath::Vec3Length(dragDir) * 2.0f;
        Renderer().DrawLine(startPos, startPos + BMath::Vec3Normalize(dragDir) * lineLength, {1.0f,0.0f, 0.0f, 1.0f});
    }
}
void TestGame::Update(f32 deltaTime)
{
    (void)deltaTime;
    if(BitEngine::BInput::IsMouseButtonPressed(BitEngine::BInput::MOUSE_BUTTON_RIGHT)) 
    {
        BitEngine::BInput::GetMousePosition(&initialMouseX, &initialMouseY);

        isDragging = true;
        BIT_LOG_DEBUG("Mouse RIGHT pressed - started dragging at %d, %d", initialMouseX, initialMouseY);
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
        m_Physics2DSystem->ApplyImpulse(whiteBall, impulse);

        isDragging = false;
    }
    if(BitEngine::BInput::IsMouseButtonDown(BitEngine::BInput::MOUSE_BUTTON_LEFT))
    {
        i32 screenX, screenY;
        BitEngine::BInput::GetMousePosition(&screenX, &screenY);

        BMath::Vec3 mousePos = ScreenToWorldCoords((f32)screenX, (f32)screenY);
        mousePos.z = -7.0f;

        BIT_LOG_DEBUG("Mouse world pos: %f, %f", mousePos.x, mousePos.y);

        auto circleEntity= Entities().CreateEntity();
        circleEntity.AddComponent<BitEngine::TransformComponent>(
                mousePos,
                (0.0f),
                (0.0f)
                );
        auto& circleSprite = circleEntity.AddComponent<BitEngine::Circle2DComponent>(3.0f);
        circleSprite.Color = {1.0f, 0.4f, 0.0f, 1.0f};
        circleEntity.AddComponent<BitEngine::Circle2DColliderComponent>(3.0f);
        circleEntity.AddComponent<BitEngine::Rigid2DBodyComponent>(10.0f);
    }

    auto cameraPos = Camera().GetActiveCamera()->Position;
    BIT_LOG_DEBUG("Camera pos: %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
    BIT_LOG_DEBUG("World size: %f x %f", m_WorldWidth, m_WorldHeight);

}

// void TestGame::OnJump(const BitEngine::Entity& entity)
// {
//     (void)
//     BIT_LOG_DEBUG("Player JUMP..!");
// }

void TestGame::SetupInput()
{
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
