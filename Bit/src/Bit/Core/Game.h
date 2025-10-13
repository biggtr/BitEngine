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
struct GameSystems
{
    Renderer2D* renderer2D;
    Renderer* renderer3D;
    EntityManager* entityManager;
    AssetManager* assetManager;
    CameraManager* cameraManager;
};

extern Game* CreateGame();
class Game
{
public:
    Game(){};
    virtual ~Game() = default;
    ApplicationConfig m_AppConfig;
protected:
    Renderer2D* m_Renderer2D = nullptr;
    Renderer* m_Renderer3D = nullptr;
    EntityManager* m_ECS = nullptr;
    AssetManager* m_AssetManager = nullptr;
    CameraManager* m_CameraManager = nullptr;
    

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
                DegToRad(45.0f), m_AppConfig.width / (f32)m_AppConfig.height, 0.1f, 100.0f);
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
        ActiveWorldCamera->SetType(CAMERA_TYPE::ORTHO);

        Initialize();
        SetupInput();
        return true;
    }
    virtual b8 OnUpdate(f64 deltaTime)
    {
        m_Animation2DSystem->Update(deltaTime);
        m_CollisionSystem->Update();
        m_Physics2DSystem->Update(deltaTime);
        Update(deltaTime);
        return true;
    }
    virtual b8 OnRender() 
    {
        // First render pass - UI (working?)
        m_Renderer2D->BeginScene(m_UIProjection);
        m_UIRenderSystem->Update(*m_Renderer2D);
        UIRender();
        m_Renderer2D->EndScene();

        // Second render pass - World (has NaN?)
        BMath::Mat4 ProjectionMatrix = ActiveWorldCamera->GetType() == CAMERA_TYPE::ORTHO 
                                        ? m_OrthoProjection : m_PerspectiveProjection;
        
        // ✅ DEBUG: Check projection matrix
        BIT_LOG_DEBUG("OrthoProj[0] = %.4f, [5] = %.4f", m_OrthoProjection.Data[0], m_OrthoProjection.Data[5]);
        for(int i = 0; i < 16; i++) {
            if(std::isnan(m_OrthoProjection.Data[i])) {
                BIT_LOG_ERROR("OrthoProjection has NaN at index %d!", i);
            }
        }
        
        BMath::Mat4 viewMatrix = ActiveWorldCamera->GetViewMatrix();
        
        // ✅ DEBUG: Check view matrix
        BIT_LOG_DEBUG("ViewMatrix[0] = %.4f", viewMatrix.Data[0]);
        for(int i = 0; i < 16; i++) {
            if(std::isnan(viewMatrix.Data[i])) {
                BIT_LOG_ERROR("ViewMatrix has NaN at index %d!", i);
            }
        }
        
        BMath::Mat4 viewProjection = ProjectionMatrix * viewMatrix;
        
        // ✅ DEBUG: Check final result
        BIT_LOG_DEBUG("ViewProjection[0] = %.4f, [4] = %.4f, [8] = %.4f, [12] = %.4f", 
                      viewProjection.Data[0], viewProjection.Data[4], viewProjection.Data[8], viewProjection.Data[12]);
        for(int i = 0; i < 16; i++) {
            if(std::isnan(viewProjection.Data[i])) {
                BIT_LOG_ERROR("ViewProjection has NaN at index %d!", i);
                // Print what went into the calculation
                BIT_LOG_ERROR("  Projection[%d] = %.4f", i, ProjectionMatrix.Data[i]);
                BIT_LOG_ERROR("  View[%d] = %.4f", i, viewMatrix.Data[i]);
            }
        }

        m_Renderer3D->BeginFrame(viewProjection);
        Render();
        m_Renderer3D->EndFrame();

        m_Renderer2D->BeginScene(viewProjection);  // ← This is where NaN comes from
        m_RenderSystem->Update(*m_Renderer2D);
        Render();
        m_Renderer2D->EndScene();

        return true;
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
        
        m_Renderer2D->SetViewport(0, 0, width, height);
    }

    BMath::Vec3 ScreenToWorldCoords(f32 screenX, f32 screenY) 
    {
        f32 ndcX = (2.0f * screenX) / m_AppConfig.width - 1.0f;
        f32 ndcY = 1.0f - (2.0f * screenY) / m_AppConfig.height; 
        
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
