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
        BMath::Vector3(1.5f, 1.5f, 0.0f),  
        BMath::Vector3(0.1f, 0.1f, 0.0f),  
        BMath::Vector3(0.0f, 0.0f, 0.0f)   
    ); 
    const auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entt1,
            Assets().GetTexture("Basic")
    );
    
    BIT_LOG_DEBUG("Sprite Width : %d, Height: %d", sprite.Width, sprite.Height);
    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
