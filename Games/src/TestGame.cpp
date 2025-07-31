#include "TestGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Math/Vector.h"
#include <cstdio>

bool TestGame::Initialize()
{
    auto entt1 = Entities().CreateEntity();
    
    Entities().AddComponent<BitEngine::CTransform>(entt1, 
        BitMath::Vector3(1.0f, 1.0f, 1.0f),  
        BitMath::Vector3(1.0f, 1.0f, 1.0f),  
        BitMath::Vector3(0.0f, 0.0f, 0.0f)   
    ); 
    printf("Hi from TestGame\n");
    
    return true;
}

void TestGame::OnRender()
{
    return;
}
void TestGame::OnUpdate(float deltaTime)
{

    return;
}
