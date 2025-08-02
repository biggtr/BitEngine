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
    Entities().AddSystem<BitEngine::RenderSystem>();
    auto entt1 = Entities().CreateEntity();
    
    Entities().AddComponent<BitEngine::CTransform>(entt1, 
        BMath::Vector3(200.0f, 200.0f, 0.0f),  
        BMath::Vector3(100.0f, 100.0f, 1.0f),  
        BMath::Vector3(0.0f, 0.0f, 0.0f)   
    ); 
    printf("Hi from TestGame\n");
    
    return true;
}

void TestGame::OnRender()
{
    Entities().GetSystem<BitEngine::RenderSystem>().Update(Renderer());
    return;
}
void TestGame::OnUpdate(float deltaTime)
{

    return;
}
