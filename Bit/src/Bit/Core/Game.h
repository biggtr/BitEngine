#pragma once 
#include "Application.h"
#include "Bit/Core/Input.h"
#include "Bit/Math/BMath.h"
#include "Bit/Renderer/CameraManager.h"
#include "Bit/Resources/AssetManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/Renderer.h"
#include "Bit/Systems/Animation2DSystem.h"
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
    Renderer2D& Render2D() { return App().GetRenderer2D(); }
    Renderer& Renderer3D() { return App().GetRenderer(); }
    EntityManager& ECS() { return App().GetEntityManager(); }
    AssetManager& Assets() { return App().GetAssetManager(); }
    CameraManager& Camera() { return App().GetCameraManager(); }

    RenderSystem* m_RenderSystem;
    UIRenderSystem* m_UIRenderSystem;
    Physics2DSystem* m_Physics2DSystem;
    CollisionSystem* m_CollisionSystem;
    Animation2DSystem* m_Animation2DSystem;
    InputSystem* m_InputSystem;

    class Camera* ActiveWorldCamera;

    BMath::Mat4 m_PerspectiveProjection;
    BMath::Mat4 m_OrthoProjection;
    BMath::Mat4 m_UIProjection;
                                       
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
        m_OrthoProjection = BMath::Mat4Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );

        m_PerspectiveProjection = BMath::Mat4Perspective(
                DegToRad(45.0f), appConfig.width / (f32)appConfig.height, 0.1f, 100.0f);
        m_UIProjection = BMath::Mat4Ortho(
            0.0f,  appConfig.width, 
            appConfig.height, 0.0f,
            -1.0f, 1.0f         
        );
    }
    virtual b8 OnInitialize() 
    {
        SetupGameSize();

        ECS().AddSystem<RenderSystem>();
        ECS().AddSystem<UIRenderSystem>();
        ECS().AddSystem<Physics2DSystem>();
        ECS().AddSystem<CollisionSystem>();
        ECS().AddSystem<Animation2DSystem>();
        ECS().AddSystem<InputSystem>();

        m_RenderSystem = ECS().GetSystem<RenderSystem>();
        m_UIRenderSystem = ECS().GetSystem<UIRenderSystem>();
        m_Physics2DSystem = ECS().GetSystem<Physics2DSystem>();
        m_CollisionSystem = ECS().GetSystem<CollisionSystem>();
        m_Animation2DSystem = ECS().GetSystem<Animation2DSystem>();
        m_InputSystem = ECS().GetSystem<InputSystem>();

        ActiveWorldCamera = Camera().GetDefaultCamera();
        ActiveWorldCamera->SetType(CAMERA_TYPE::ORTHO);

        Initialize();
        SetupInput();
        return true;
    }
    virtual void OnUpdate(f64 deltaTime)
    {
        m_Animation2DSystem->Update(deltaTime);
        m_CollisionSystem->Update();
        m_Physics2DSystem->Update(deltaTime);
        Update(deltaTime);
    }
    virtual void OnRender() 
    {

        Render2D().BeginScene(m_UIProjection);
        m_UIRenderSystem->Update(Render2D());
        UIRender();
        Render2D().EndScene();
        // BMath::Mat4 ProjectionMatrix = ActiveWorldCamera->GetType() == CAMERA_TYPE::ORTHO ? m_OrthoProjection : m_PerspectiveProjection;
        
        BMath::Mat4 viewProjection = m_PerspectiveProjection * ActiveWorldCamera->GetViewMatrix();
        Render2D().BeginScene(viewProjection);
        m_RenderSystem->Update(Render2D());
        Render();
        Render2D().EndScene();

        Renderer3D().BeginFrame(viewProjection);
        Render();
        Renderer3D().EndFrame();


    }
    void OnWindowResize(u16 width, u16 height)
    {
        f32 aspectRatio = (f32)width / (f32)height;
        f32 halfHeight = VIEWPORT_HEIGHT / 2.0f;
        f32 halfWidth = halfHeight * aspectRatio;

        m_OrthoProjection = BMath::Mat4Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );
        
        
        m_UIProjection = BMath::Mat4Ortho(
            0, width,
            height, 0,
            -1.0f, 1.0f
        );
        
        Render2D().SetViewport(0, 0, width, height);
    }

    BMath::Vec3 ScreenToWorldCoords(f32 screenX, f32 screenY) 
    {
        f32 ndcX = (2.0f * screenX) / appConfig.width - 1.0f;
        f32 ndcY = 1.0f - (2.0f * screenY) / appConfig.height; 
        
        BMath::Mat4 viewProjection = m_OrthoProjection * ActiveWorldCamera->GetViewMatrix();
        BMath::Mat4 invViewProjection = BMath::Mat4Inverse(viewProjection);

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
