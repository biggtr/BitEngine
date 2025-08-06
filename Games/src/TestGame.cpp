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
    auto entt2 = Entities().CreateEntity(); 
    Entities().AddComponent<BitEngine::CTransform>(entt1, 
        BMath::vec3(0.0f, 0.0f, 0.0f),  
        BMath::vec3(50.5f, 50.5f, 0.0f),  
        BMath::vec3(0.0f, 0.0f, 0.0f)   
    ); 
    const auto& sprite = Entities().AddComponent<BitEngine::CSprite>(entt1,
            Assets().GetTexture("Basic"),
            BMath::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    
    Entities().AddComponent<BitEngine::CTransform>(entt2, 
        BMath::vec3(0.0f, 0.0f, 5.0f),  
        BMath::vec3(100.4f, 100.4f, 0.0f),  
        BMath::vec3(0.0f, 0.0f, 0.0f)   
    ); 
    const auto& sprite2 = Entities().AddComponent<BitEngine::CSprite>(entt2,
            Assets().GetTexture("Basic"),
            BMath::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    BIT_LOG_DEBUG("Sprite 1 Width : %d, Height: %d", sprite.Width, sprite.Height);
    BIT_LOG_DEBUG("Sprite 2 Width : %d, Height: %d", sprite2.Width, sprite2.Height);
    return true;
}

void TestGame::Render()
{
}
void TestGame::Update(float deltaTime)
{

}
