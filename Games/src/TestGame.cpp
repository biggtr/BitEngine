#include "TestGame.h"
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Math/Vector.h"
#include "Bit/Systems/RenderSystem.h"

bool TestGame::Initialize()
{
    Assets().AddTexture("Basic", "assets/textures/Projectutumno.png");
    // Assets().AddTexture("Basic", "assets/textures/icon_chest.png");
    
    auto entity = Entities().CreateEntity();
    auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entity,
                Assets().GetTexture("Basic")
                );
    sprite.FrameWidth = sprite.FrameHeight =  32;
    sprite.CurrentFrame = 400;
    Entities().AddComponent<BitEngine::CTransform>(entity, 
                BMath::Vec3(300.0f, 300.0f, 0.0f),  
                BMath::Vec3(400.0f, 200.1f, 0.0f), 
                BMath::Vec3(0.0f, 0.0f, 0.0f)    
                ); 
    // for(u32 i = 0; i < 10; ++i)
    // {
    //     auto entity = Entities().CreateEntity();
    //     float x = (i % 50) * 50.0f;
    //     float y = ((float)i / 50) * 50.0f;
    //     auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entity,
    //                 Assets().GetTexture("Basic")
    //                 );
    //     sprite.FrameWidth = sprite.FrameHeight =  32;
    //     sprite.CurrentFrame = 400;
    //     // Entities().AddComponent<BitEngine::CBoxCollider>(entity, 
    //     //                 BMath::Vec3(15.0f, 32.0f, 0.0f)
    //     //             );
    //     Entities().AddComponent<BitEngine::CTransform>(entity, 
    //                 BMath::Vec3(x, y, 0.0f),  
    //                 BMath::Vec3(450.0f, 450.1f, 0.0f), 
    //                 BMath::Vec3(0.0f, 0.0f, 0.0f)    
    //                 ); 
    // }

    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
