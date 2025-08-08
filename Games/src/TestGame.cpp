#include "TestGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Math/Vector.h"
#include "Bit/Systems/RenderSystem.h"
#include <cstdio>

bool TestGame::Initialize()
{
    Assets().AddTexture("Basic", "assets/textures/icon_chest.png");
    auto entt1 = Entities().CreateEntity();
    Entities().AddComponent<BitEngine::CTransform>(entt1, 
        BMath::Vec3(100.0f, 0.0f, 0.0f),  
        BMath::Vec3(50.0f, 100.0f, 0.0f),  
        BMath::Vec3(0.0f, 0.0f, 0.0f)   
    ); 
    const auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entt1,
            Assets().GetTexture("Basic"),
            BMath::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    
    BIT_LOG_DEBUG("Sprite 1 Width : %d, Height: %d", sprite.Width, sprite.Height);
    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
