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
    
    for(u32 i = 0; i < 10000; ++i)
    {
        auto entity = Entities().CreateEntity();
        float x = (i % 50) * 50.0f;
        float y = ((float)i / 50) * 50.0f;
        auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entity,
                    Assets().GetTexture("Basic")
                    );
        sprite.FrameWidth = sprite.FrameHeight =  32;
        sprite.CurrentFrame = 300;
        Entities().AddComponent<BitEngine::CBoxCollider>(entity, 
                        BMath::Vec3(15.0f, 32.0f, 0.0f)
                    );
        Entities().AddComponent<BitEngine::CTransform>(entity, 
                    BMath::Vec3(x, y, 0.0f),  
                    BMath::Vec3(50.0f, 50.1f, 0.0f), 
                    BMath::Vec3(0.0f, 0.0f, 0.0f)    
                    ); 
    }

    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
