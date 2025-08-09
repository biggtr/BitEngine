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
            BMath::Vec3(400.0f, 400.0f, 0.0f),   
            BMath::Vec3(100.2f, 200.1f, 0.0f),     
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
