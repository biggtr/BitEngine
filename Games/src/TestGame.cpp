#include "TestGame.h"
#include "Bit/Math/Vector.h"
#include "Bit/Scene/Compontents.h"

void TestGame::Initialize()
{
    Assets().AddTexture("Player", "assets/textures/player.png");
    auto player = Entities().CreateEntity();
    player.AddComponent<BitEngine::CTransform>(
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(100.0f, 100.0f, 0.0f),
            BMath::Vec3(0.0f, 0.0f, 0.0f)
            );
    auto& playersprite = player.AddComponent<BitEngine::CSprite>(
            Assets().GetTexture("Player") 
            );

    playersprite.FrameHeight = playersprite.FrameWidth = 32;
    playersprite.CurrentFrame = 4;

}

void TestGame::Render()
{

}
void TestGame::Update(f32 deltaTime)
{
    f32 movementSpeed = 1150.0f * deltaTime;
    BMath::Vec3 movement = Camera().GetActiveCamera()->Position;
    if(Inputs().IsKeyDown(BitEngine::KEY_A))
    {
        movement.x -= movementSpeed;
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_D))
    {
        movement.x += movementSpeed; 
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_W))
    {
        movement.y += movementSpeed;
    }
    if(Inputs().IsKeyDown(BitEngine::KEY_S))
    {
        movement.y -= movementSpeed;
    }
    Camera().SetPosition(movement);
}
