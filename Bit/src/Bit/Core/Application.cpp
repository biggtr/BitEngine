#include "Application.h"
#include "Bit/Font/Font.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Profiler/Profiler.h"
#include "Bit/Resources/AssetManager.h"
#include "Bit/Core/Core.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/RenderSystem.h"
#include "Bit/Systems/MovementSystem.h"
#include "Bit/UI/BitUI.h"
#include "Game.h"
#include "Bit/Core/Logger.h"
#include "Platform.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include <cstddef>

namespace BitEngine
{
Application* Application::s_Instance = nullptr;
b8 Application::Create(Game *gameInstance)
{
    if(!Logger::Initialize())
    {
        return false;
    }
    if(!BPhysics2D::Initialize())
    {
        return false;
    }
    if(s_Instance)
    {
        BIT_LOG_ERROR("You Cannot Create More Than One Application Instance At A Time");
        return false;
    }
    s_Instance = new Application();
    // Init All systems inhere 
    if(!s_Instance->Initialize(gameInstance->appConfig))
    {
        BIT_LOG_ERROR("Failed to initialize application");
        return false;
    }
    s_Instance->m_GameInstance = gameInstance;
    s_Instance->m_IsRunning = true;
    s_Instance->m_IsSuspended = false;

    if(!s_Instance->m_GameInstance->OnInitialize())
    {
        BIT_LOG_ERROR("Failed to initialize game instance");
        return false;
    }

    return true;
}

bool Application::Initialize(ApplicationConfig appCfg)
{
    m_Initialized = true;
    
    m_Window = BitEngine::Platform::Create(appCfg.width, appCfg.height, appCfg.name);
    if(!m_Window)
    {
        BIT_LOG_ERROR("Failed to create window");
        return false;
    }
    m_Window->Initialize();
    
    m_Renderer2D = new Renderer2D();
    if(!m_Renderer2D->Initialize())
    {
        BIT_LOG_ERROR("Failed to create renderer");
        return false;
    }

    m_EntityManager = new EntityManager();
    if(!m_EntityManager)
    {
        BIT_LOG_ERROR("Failed to create EntityManager");
        return false;
    }

    m_AssetManager = new AssetManager();
    if(!m_AssetManager)
    {
        BIT_LOG_ERROR("Failed to create AssetManager");
        return false;
    }

    m_CameraManager = new CameraManager();
    if(!m_CameraManager)
    {
        BIT_LOG_ERROR("Failed to create CameraManager");
        return false;
    }
    m_EventManager = new EventManager();
    if(!m_EventManager->Initialize())
    {
        BIT_LOG_ERROR("Failed to create EventManager");
        return false;
    }
    m_EventManager->Register(EVENT_CODE_KEY_PRESSED, 0, ApplicationOnKey);
    m_EventManager->Register(EVENT_CODE_APPLICATION_QUIT, 0, ApplicationOnEvent);
    m_EventManager->Register(EVENT_CODE_WINDOW_RESIZED , 0, ApplicationOnEvent);

    if(!BInput::Initialize())
    {
        BIT_LOG_ERROR("Failed to create Input System");
        return false;
    }
    if(!BitUI::Initialize())
    {
        BIT_LOG_ERROR("Failed to create Input System");
        return false;
    }
    return true;
}

void Application::Run()
{
    s_Instance->m_Time.Reset();
    // f64 FPS = 0.0f;
    while(s_Instance->m_IsRunning)
    {
        s_Instance->m_Time.Update();
        f64 deltaTime = s_Instance->m_Time.GetDeltaTime();
        // FPS = 1.0f / deltaTime;

        // BIT_LOG_DEBUG("FPS: %.2f", FPS);
        if(!s_Instance->m_IsSuspended && s_Instance->m_GameInstance)
        {
            
            s_Instance->m_EntityManager->Update();
        
            s_Instance->m_GameInstance->OnUpdate(deltaTime);
            BInput::Update();
        

            s_Instance->m_Renderer2D->SetClearColor(BMath::Vec4(0.23f, 0.0f, 1.0, 1.0));
            s_Instance->m_Renderer2D->Clear();
            s_Instance->m_GameInstance->OnRender();

            s_Instance->m_Window->OnUpdate();
        }
    }
}
Application::~Application()
{
    delete m_CameraManager;
    delete m_AssetManager;
    delete m_EntityManager;
    delete m_Renderer2D;
    delete m_Window;
    delete m_EventManager;
    
    m_CameraManager = nullptr;
    m_AssetManager = nullptr;
    m_EntityManager = nullptr;
    m_Renderer2D = nullptr;
    m_Window = nullptr;
    m_EventManager = nullptr;
}

b8 Application::Shutdown()
{
    if(!s_Instance)
        return false;

    BitUI::Shutdown();
    BInput::Shutdown();
    s_Instance->m_EventManager->UnRegister(EVENT_CODE_APPLICATION_QUIT, 0, ApplicationOnEvent);
    s_Instance->m_EventManager->UnRegister(EVENT_CODE_WINDOW_RESIZED , 0, ApplicationOnEvent);
    s_Instance->m_EventManager->UnRegister(EVENT_CODE_KEY_PRESSED, 0, ApplicationOnKey);
    s_Instance->m_EventManager->Shutdown();
    s_Instance->m_AssetManager->ClearTextures();
    s_Instance->m_Renderer2D->Shutdown();
    s_Instance->m_Window->Shutdown();
    s_Instance->m_IsRunning = false;
    delete s_Instance;
    s_Instance = nullptr;

    Logger::Shutdown();
    BPhysics2D::Shutdown();
    return true;
}
b8 Application::ApplicationOnEvent(u16 code, void* sender, void* listener, EventContext data)
{
    switch (code) 
    {
        case EVENT_CODE_APPLICATION_QUIT:
            {
                BIT_LOG_INFO("Application is shutting down..!");
                s_Instance->m_IsRunning = false;
                return true;
            }
        case EVENT_CODE_WINDOW_RESIZED:
            {
                u16 width = data.U16[0];
                u16 height = data.U16[1];
                BIT_LOG_INFO("Updated Window size -> Width : %d, Height: %d", width, height);
                s_Instance->m_GameInstance->OnWindowResize(width, height);
            }
    }
    return false;
}
b8 Application::ApplicationOnKey(u16 code, void* sender, void* listener, EventContext data)
{
    switch (code) 
    {
        case EVENT_CODE_KEY_PRESSED:
            {
                u16 key = data.U16[0];
                switch (key) 
                {
                    case BInput::KEY_ESCAPE:
                    {
                        EventContext zerodata = {};
                        EventManager::EventFire(EVENT_CODE_APPLICATION_QUIT, 0, zerodata);
                    }
                }
                return true;
            }
    }
    return false;
}
}
