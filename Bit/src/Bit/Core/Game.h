#pragma once 
#include "Application.h"
#include "Bit/Assets/AssetManager.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Systems/MovementSystem.h"
#include "Bit/Systems/RenderSystem.h"
namespace BitEngine 
{
extern Game* CreateGame();
class Game
{
public:
    ApplicationConfig appConfig;
protected:
    Application& App() { return Application::GetApplication(); }
    Renderer2D& Renderer() { return App().GetRenderer(); }
    EntityManager& Entities() { return App().GetEntityManager(); }
    AssetManager& Assets() { return App().GetAssetManager(); }
public:
    Game(){}
    virtual ~Game(){}
    virtual bool OnInitialize() 
    {
        Entities().AddSystem<RenderSystem>();
        Entities().AddSystem<MovementSystem>();


        return Initialize();
    }
    virtual void OnUpdate(float deltaTime)
    {
        // Entities().GetSystem<MovementSystem>().Update(deltaTime);

        Update(deltaTime);
    }
    virtual void OnRender() 
    {
        Entities().GetSystem<RenderSystem>().Update(Renderer());
        Render();
    }

protected:
    virtual bool Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0; 
};
}
