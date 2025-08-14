#pragma once 
#include "Application.h"
#include "Bit/Resources/AssetManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Systems/Animation2DSystem.h"
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
        Entities().AddSystem<Animation2DSystem>();


        auto camera = Entities().CreateEntity();
        CCamera cameraComponent = Entities().AddComponent<CCamera>(camera, 
                BMath::Vec3(0.0f, 0.0f, 10.0f),
                DegToRad(0.0f)
                );

        Camera().SetActiveCamera(&cameraComponent);
        return Initialize();
    }
    virtual void OnUpdate(f64 deltaTime)
    {
        Entities().GetSystem<CameraSystem>().Update(deltaTime);
        Entities().GetSystem<Animation2DSystem>().Update(deltaTime);

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
