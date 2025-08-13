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
        sprite.CurrentFrame = 300;
        Entities().AddComponent<BitEngine::CBoxCollider>(entity, 
                32, 32
                );
        Entities().AddComponent<BitEngine::CTransform>(entity, 
                BMath::Vec3(100.0f, 300.0f, 0.0f),  
                BMath::Vec3(150.0f, 150.1f, 0.0f), 
                BMath::Vec3(0.0f, 0.0f, 0.0f)    
                ); 

    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
