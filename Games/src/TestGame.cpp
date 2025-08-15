#include "TestGame.h"
#include "Bit/Scene/Compontents.h"

b8 TestGame::Initialize()
{
    Assets().AddTexture("Basic", "assets/textures/Projectutumno.png");
    // for(u32 i = 0; i < 10000; ++i)
    // {
    //     auto entity = Entities().CreateEntity();
    //     f32 x = (i % 50) * 50.0f;
    //     f32 y = ((f32)i / 50) * 50.0f;
    //     auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entity,
    //                 Assets().GetTexture("Basic")
    //                 );
    //     sprite.FrameWidth = sprite.FrameHeight =  32;
    //     sprite.CurrentFrame = 400;
    //     Entities().AddComponent<BitEngine::CTransform>(entity, 
    //                 BMath::Vec3(x, y, 0.0f),  
    //                 BMath::Vec3(50.0f, 50.1f, 0.0f), 
    //                 BMath::Vec3(0.0f, 0.0f, 0.0f)    
    //                 ); 
    //
    // }
    auto sword = Entities().CreateEntity();
    auto& swordSprite = Entities().AddComponent<BitEngine::CSprite>(sword,
            Assets().GetTexture("Basic")
            );
    swordSprite.FrameHeight = swordSprite.FrameWidth = 32;
    swordSprite.CurrentFrame = 400;
    
    Entities().AddComponent<BitEngine::CTransform>(sword, 
                BMath::Vec3(100.0f, 100.0f, 0.0f),  
                BMath::Vec3(100.0f, 100.0f, 0.0f), 
                BMath::Vec3(0.0f, 0.0f, 0.0f)    
                ); 

    auto entity = Entities().CreateEntity();
    auto& entitySprite = Entities().AddComponent<BitEngine::CSprite>(entity,
            Assets().GetTexture("Basic")
            );
    entitySprite.FrameHeight = entitySprite.FrameWidth = 32;
    entitySprite.CurrentFrame = 300;
    
    Entities().AddComponent<BitEngine::CTransform>(entity, 
                BMath::Vec3(200.0f, 200.0f, 0.0f),  
                BMath::Vec3(100.0f, 100.0f, 0.0f), 
                BMath::Vec3(0.0f, 0.0f, 0.0f)    
                ); 
    Entities().KillEntity(entity);
    return true;
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
        BIT_LOG_DEBUG("Moving in -x : %.2f", movement.x);
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
