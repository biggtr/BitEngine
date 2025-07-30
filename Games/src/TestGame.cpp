#include "TestGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Entity.h"
#include <cstdio>

bool TestGame::Initialize()
{
    auto entt1 = Entities().CreateEntity();
    
    auto entt2 = Entities().CreateEntity();
    
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
