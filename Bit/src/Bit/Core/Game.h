#pragma once 
#include "Application.h"
#include "Bit/Core/Input.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Math/BMath.h"
#include "Bit/Particles/ParticleSystem.h"
#include "Bit/Renderer/CameraManager.h"
#include "Bit/Resources/AssetManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/Renderer.h"
#include "Bit/ECS/Systems/Animation2DSystem.h"
#include "Bit/ECS/Systems/CollisionSystem.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/MovementSystem.h"
#include "Bit/ECS/Systems/Physics2DSystem.h"
#include "Bit/ECS/Systems/RenderSystem.h"
#include "Bit/ECS/Systems/UIRenderSystem.h"
#include "Bit/Resources/GeometryManager.h"
#include "Bit/Resources/MaterialManager.h"
#include "Bit/Tiles/TileMap.h"
namespace BitEngine 
{
struct GameSystems
{
    Renderer2D* renderer2D;
    Renderer* renderer3D;
    EntityManager* entityManager;
    AssetManager* assetManager;
    CameraManager* cameraManager;
    ParticleSystem* particleSystem;
};

extern Game* CreateGame();
class Game
{
public:
    Game(){}
    virtual ~Game()
    {
        if(m_TileEditor)
            delete m_TileEditor;
    }
    ApplicationConfig m_AppConfig;
protected:
    Renderer2D* m_Renderer2D = nullptr;
    Renderer* m_Renderer3D = nullptr;
    EntityManager* m_ECS = nullptr;
    AssetManager* m_AssetManager = nullptr;
    CameraManager* m_CameraManager = nullptr;
    ParticleSystem* m_ParticleSystem = nullptr;
    MaterialManager* m_MaterialManager = nullptr;
    GeometryManager* m_GeometryManager = nullptr;
    

    RenderSystem* m_RenderSystem;
    UIRenderSystem* m_UIRenderSystem;
    Physics2DSystem* m_Physics2DSystem;
    CollisionSystem* m_CollisionSystem;
    Animation2DSystem* m_Animation2DSystem;
    InputSystem* m_InputSystem;
    TileEditor* m_TileEditor;

    class Camera* ActiveWorldCamera;

    BMath::Mat4 m_PerspectiveProjection;
    BMath::Mat4 m_OrthoProjection;
    BMath::Mat4 m_UIProjection;
                                       
    const f32 VIEWPORT_HEIGHT = 100.0f;
    f32 m_WorldHeight;
    f32 m_WorldWidth;
public:

    void SetupGameSize()
    {
        f32 aspectRatio = m_AppConfig.width / (f32)m_AppConfig.height;
        f32 halfHeight = VIEWPORT_HEIGHT / 2.0f;
        f32 halfWidth = halfHeight * aspectRatio;
        m_OrthoProjection = BMath::Mat4Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );

        m_PerspectiveProjection = BMath::Mat4Perspective(
                BMath::DegToRad(45.0f), m_AppConfig.width / (f32)m_AppConfig.height, 0.1f, 100.0f);
        m_UIProjection = BMath::Mat4Ortho(
            0.0f,  m_AppConfig.width, 
            m_AppConfig.height, 0.0f,
            -1.0f, 1.0f         
        );
    }
    virtual b8 OnInitialize(GameSystems services) 
    {
        SetupGameSize();

        m_Renderer2D = services.renderer2D;
        m_Renderer3D = services.renderer3D;
        m_ECS = services.entityManager;
        m_AssetManager = services.assetManager;
        m_CameraManager = services.cameraManager;
        m_ParticleSystem = services.particleSystem;
        m_MaterialManager = m_Renderer3D->GetMaterialManager();
        m_GeometryManager = m_Renderer3D->GetGeometryManager();

        services.entityManager->AddSystem<RenderSystem>();
        services.entityManager->AddSystem<UIRenderSystem>();
        services.entityManager->AddSystem<Physics2DSystem>();
        services.entityManager->AddSystem<CollisionSystem>();
        services.entityManager->AddSystem<Animation2DSystem>();
        services.entityManager->AddSystem<InputSystem>();

        m_RenderSystem      = services.entityManager->GetSystem<RenderSystem>();
        m_UIRenderSystem    = services.entityManager->GetSystem<UIRenderSystem>();
        m_Physics2DSystem   = services.entityManager->GetSystem<Physics2DSystem>();
        m_CollisionSystem   = services.entityManager->GetSystem<CollisionSystem>();
        m_Animation2DSystem = services.entityManager->GetSystem<Animation2DSystem>();
        m_InputSystem       = services.entityManager->GetSystem<InputSystem>();

        ActiveWorldCamera = services.cameraManager->GetDefaultCamera();
        ActiveWorldCamera->SetPosition(BMath::Vec3(0.0f, 0.0f, 10.0f)); 
        ActiveWorldCamera->SetType(CAMERA_TYPE::ORTHO);

        m_TileEditor = new TileEditor(m_Renderer2D);
        m_TileEditor->Initialize();

        Initialize();
        SetupInput();
        return true;
    }
    virtual b8 OnUpdate(f64 deltaTime)
    {
        Update(deltaTime);
        m_Animation2DSystem->Update(deltaTime);
        m_CollisionSystem->Update();
        m_Physics2DSystem->Update(deltaTime);
        if (m_TileEditor)
        {
            BMath::Mat4 ProjectionMatrix = ActiveWorldCamera->GetType() == CAMERA_TYPE::ORTHO 
                ? m_OrthoProjection : m_PerspectiveProjection;
            BMath::Mat4 viewMatrix = ActiveWorldCamera->GetViewMatrix();
            BMath::Mat4 viewProjection = ProjectionMatrix * viewMatrix;

            m_TileEditor->Update(deltaTime, ActiveWorldCamera, viewProjection);
        }
        m_ParticleSystem->OnUpdate(deltaTime);
        return true;
    }
    virtual b8 OnRender() 
    {
        m_Renderer3D->SetClearColor({1.0f, 0.0f, 0.0f, 1.0f});
        m_Renderer3D->Clear();
        BMath::Mat4 ProjectionMatrix = ActiveWorldCamera->GetType() == CAMERA_TYPE::ORTHO 
            ? m_OrthoProjection : m_PerspectiveProjection;
        BMath::Mat4 viewMatrix = ActiveWorldCamera->GetViewMatrix();
        BMath::Mat4 viewProjection = ProjectionMatrix * viewMatrix;

        m_Renderer2D->BeginScene(viewProjection);
        if (m_TileEditor)
        {
            m_TileEditor->Render(m_Renderer2D, viewProjection);
        }
        Render2D();
        m_ParticleSystem->OnRender(m_Renderer2D);
        m_RenderSystem->Update(*m_Renderer2D);
        m_Renderer2D->EndScene();

        m_Renderer3D->BeginFrame(viewProjection);
        Render3D();
        m_Renderer3D->EndFrame();


        m_Renderer2D->BeginScene(m_UIProjection);
        RenderUI();
        m_UIRenderSystem->Update(*m_Renderer2D);
        m_Renderer2D->EndScene();

        return true;
    }
    void OnWindowResize(u16 width, u16 height)
    {
        m_AppConfig.width = width;
        m_AppConfig.height = height;

        f32 aspectRatio = (f32)width / (f32)height;
        f32 halfHeight = VIEWPORT_HEIGHT / 2.0f;
        f32 halfWidth = halfHeight * aspectRatio;

        m_OrthoProjection = BMath::Mat4Ortho(
            -halfWidth, halfWidth, 
            -halfHeight, halfHeight,
            -100.0f, 100.0f         
        );
        m_PerspectiveProjection = BMath::Mat4Perspective(
            BMath::DegToRad(45.0f), 
            aspectRatio,
            0.1f, 100.0f
        );
        
        m_UIProjection = BMath::Mat4Ortho(
            0, width,
            height, 0,
            -1.0f, 1.0f
        );
        
        if(m_Renderer2D)
        {
            m_Renderer2D->SetViewport(0, 0, width, height);
        }
        if(m_TileEditor)
        {
            m_TileEditor->SetScreenSize(width, height);
        }
    }

    BMath::Vec3 ScreenToWorldCoords(f32 screenX, f32 screenY) 
    {
        f32 ndcX = (2.0f * (screenX + 0.5f)) / m_AppConfig.width - 1.0f;
        f32 ndcY = 1.0f - (2.0f * (screenY + 0.5f)) / m_AppConfig.height; 
        
        BMath::Mat4 viewProjection = m_OrthoProjection * ActiveWorldCamera->GetViewMatrix();
        BMath::Mat4 invViewProjection = BMath::Mat4Inverse(viewProjection);

        BMath::Vec4 worldPos =  invViewProjection * BMath::Vec4(ndcX, ndcY, 0.0f, 1.0f);
        
        return BMath::Vec4ToVec3(worldPos);
    }
protected:
    virtual void Initialize() = 0;
    virtual void SetupInput(){} 
    virtual void Update(float deltaTime) = 0;
    virtual void Render2D() {}; 
    virtual void Render3D() {}; 
    virtual void RenderUI() {}; 
};
}
