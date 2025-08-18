#pragma once 
#include "Application.h"
#include "Bit/Core/Input.h"
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
#include "Bit/Systems/InputSystem.h"
#include "Bit/Systems/MovementSystem.h"
#include "Bit/Systems/RenderSystem.h"
#include "Bit/Systems/UIRenderSystem.h"
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
    Input& Inputs() { return App().GetInput(); }

    RenderSystem* m_RenderSystem;
    UIRenderSystem* m_UIRenderSystem;
    MovementSystem* m_MovementSystem;
    CameraSystem* m_CameraSystem;
    Animation2DSystem* m_Animation2DSystem;
    InputSystem* m_InputSystem;
public:
    Game(){}
    virtual ~Game(){}
    virtual b8 OnInitialize() 
    {
        Entities().AddSystem<RenderSystem>();
        Entities().AddSystem<UIRenderSystem>();
        Entities().AddSystem<MovementSystem>();
        Entities().AddSystem<CameraSystem>(&Camera());
        Entities().AddSystem<Animation2DSystem>();
        Entities().AddSystem<InputSystem>(&Inputs());

        m_RenderSystem = Entities().GetSystem<RenderSystem>();
        m_UIRenderSystem = Entities().GetSystem<UIRenderSystem>();
        m_MovementSystem = Entities().GetSystem<MovementSystem>();
        m_CameraSystem = Entities().GetSystem<CameraSystem>();
        m_Animation2DSystem = Entities().GetSystem<Animation2DSystem>();
        m_InputSystem = Entities().GetSystem<InputSystem>();

        auto camera = Entities().CreateEntity();
        CCamera cameraComponent = Entities().AddComponent<CCamera>(camera, 
                BMath::Vec3(0.0f, 0.0f, 10.0f),
                DegToRad(0.0f)
                );

        Camera().SetActiveCamera(&cameraComponent);
        Initialize();
        SetupInput();
        return true;
    }
    virtual void OnUpdate(f64 deltaTime)
    {
        m_CameraSystem->Update(deltaTime);
        m_Animation2DSystem->Update(deltaTime);
        m_InputSystem->Update();
        Update(deltaTime);
    }
    virtual void OnRender() 
    {
        Renderer().BeginScene(BMath::Mat4::Identity());
        m_UIRenderSystem->Update(Renderer());
        Renderer().EndScene();

        Renderer().BeginScene(Camera().GetActiveCamera()->ViewMatrix);
        m_RenderSystem->Update(Renderer());
        Renderer().EndScene();

        // Renderer().BeginScene(Camera().GetActiveCamera()->ViewMatrix);
        // Render();
        // Renderer().EndScene();
    }

protected:
    virtual void Initialize() = 0;
    virtual void SetupInput() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0; 
};
}
