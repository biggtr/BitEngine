#pragma once 
#include "Application.h"
#include "Bit/Assets/AssetManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Systems/CameraSystem.h"
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
    CameraManager& Camera() { return App().GetCameraManager(); }
public:
    Game(){}
    virtual ~Game(){}
    virtual bool OnInitialize() 
    {
        Entities().AddSystem<RenderSystem>();
        Entities().AddSystem<MovementSystem>();
        Entities().AddSystem<CameraSystem>(&Camera());


        float width = (float)appConfig.width;
        float height = (float)appConfig.height;
        auto camera = Entities().CreateEntity();
        BIT_LOG_DEBUG("Width: %d, Height : %d", appConfig.width, appConfig.height);
        CCamera cameraComponent = Entities().AddComponent<CCamera>(camera, 
                BMath::Vec3(0.0f, 0.0f, 10.0f),
                BMath::Vec3(0.0f, 0.0f, 1.0f),
                true, -width / 2.0f, width / 2.0f,
                -height / 2.0f, height / 2.0f,
                -100.0f, 100.0f
                );

        Camera().SetActiveCamera(&cameraComponent);
        return Initialize();
    }
    virtual void OnUpdate(float deltaTime)
    {
        Entities().GetSystem<CameraSystem>().Update(deltaTime);

        Update(deltaTime);
    }
    virtual void OnRender() 
    {
        Renderer().BeginScene(Camera().GetActiveCamera());
        Entities().GetSystem<RenderSystem>().Update(Renderer());
        Render();
        Renderer().EndScene();
    }

protected:
    virtual bool Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0; 
};
}
