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
#include "Bit/Systems/CollisionSystem.h"
#include "Bit/Systems/InputSystem.h"
#include "Bit/Systems/MovementSystem.h"
#include "Bit/Systems/Physics2DSystem.h"
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

    RenderSystem* m_RenderSystem;
    UIRenderSystem* m_UIRenderSystem;
    Physics2DSystem* m_Physics2DSystem;
    CollisionSystem* m_CollisionSystem;
    CameraSystem* m_CameraSystem;
    Animation2DSystem* m_Animation2DSystem;
    InputSystem* m_InputSystem;

    BMath::Mat4 m_ProjectionMatrix;
    BMath::Mat4 m_UIProjectionMatrix;
                                       
    const f32 VIEWPORT_HEIGHT = 100.0f;
    f32 m_WorldHeight;
    f32 m_WorldWidth;
public:
    Game(){}
    virtual ~Game(){}

    void SetupGameSize()
    {
        f32 aspectRatio = appConfig.width / (f32)appConfig.height;
        f32 halfHeight = VIEWPORT_HEIGHT / 2.0f;
        f32 halfWidth = halfHeight * aspectRatio;
        m_ProjectionMatrix = BMath::Mat4::Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );
        m_UIProjectionMatrix = BMath::Mat4::Ortho(
            0.0f,  appConfig.width, 
            appConfig.height, 0.0f,
            -1.0f, 1.0f         
        );
    }
    virtual b8 OnInitialize() 
    {
        SetupGameSize();

        Entities().AddSystem<RenderSystem>();
        Entities().AddSystem<UIRenderSystem>();
        Entities().AddSystem<Physics2DSystem>();
        Entities().AddSystem<CollisionSystem>();
        Entities().AddSystem<CameraSystem>(&Camera());
        Entities().AddSystem<Animation2DSystem>();
        Entities().AddSystem<InputSystem>();

        m_RenderSystem = Entities().GetSystem<RenderSystem>();
        m_UIRenderSystem = Entities().GetSystem<UIRenderSystem>();
        m_Physics2DSystem = Entities().GetSystem<Physics2DSystem>();
        m_CollisionSystem = Entities().GetSystem<CollisionSystem>();
        m_CameraSystem = Entities().GetSystem<CameraSystem>();
        m_Animation2DSystem = Entities().GetSystem<Animation2DSystem>();
        m_InputSystem = Entities().GetSystem<InputSystem>();
        auto camera = Entities().CreateEntity();
        Camera2DComponent cameraComponent = Entities().AddComponent<Camera2DComponent>(camera, 
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
        m_Physics2DSystem->Update(deltaTime);
        m_CollisionSystem->Update();
        Update(deltaTime);
    }
    virtual void OnRender() 
    {

        Renderer().BeginScene(m_UIProjectionMatrix);
        m_UIRenderSystem->Update(Renderer());
        UIRender();
        Renderer().EndScene();

        BMath::Mat4 viewProjection = m_ProjectionMatrix * Camera().GetActiveCamera()->ViewMatrix;
        Renderer().BeginScene(viewProjection);
        m_RenderSystem->Update(Renderer());
        Render();
        Renderer().EndScene();


    }
    void OnWindowResize(u16 width, u16 height)
    {
        f32 aspectRatio = (f32)width / (f32)height;
        f32 halfHeight = VIEWPORT_HEIGHT / 2.0f;
        f32 halfWidth = halfHeight * aspectRatio;

        m_ProjectionMatrix = BMath::Mat4::Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );
        
        
        m_UIProjectionMatrix = BMath::Mat4::Ortho(
            0, width,
            height, 0,
            -1.0f, 1.0f
        );
        
        Renderer().SetViewport(0, 0, width, height);
    }

    BMath::Vec3 ScreenToWorldCoords(f32 screenX, f32 screenY) 
    {
        f32 ndcX = (2.0f * screenX) / appConfig.width - 1.0f;
        f32 ndcY = 1.0f - (2.0f * screenY) / appConfig.height; 
        
        BMath::Mat4 viewProjection = m_ProjectionMatrix * Camera().GetActiveCamera()->ViewMatrix;
        BMath::Mat4 invViewProjection = BMath::Mat4::Inverse(viewProjection);

        BMath::Vec4 worldPos =  invViewProjection * BMath::Vec4(ndcX, ndcY, 0.0f, 1.0f);
        
        return BMath::Vec4ToVec3(worldPos);
    }
protected:
    virtual void Initialize() = 0;
    virtual void SetupInput(){} 
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0; 
    virtual void UIRender() = 0; 
};
}
