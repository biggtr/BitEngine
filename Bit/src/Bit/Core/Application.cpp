#include "Application.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Physics/BPhysics.h"
#include "Game.h"
#include "Platform/Platform.h"
#include <cstdlib>

namespace BitEngine
{

Application::Application()
{
    m_Renderer3D = new Renderer();
    m_Renderer2D = new Renderer2D();
    m_EntityManager = new EntityManager();
    m_AssetManager = new AssetManager();
    m_CameraManager = new CameraManager();
    
}

Application::~Application()
{
    delete m_CameraManager;
    delete m_AssetManager;
    delete m_EntityManager;
    delete m_Renderer2D;
    delete m_Renderer3D;
    delete m_GameInstance;
}

b8 Application::Create(Game* gameInstance)
{
    u64 m_EventSystemMemReq;
    EventInitialize(&m_EventSystemMemReq, 0);
    m_EventSystem = malloc(m_EventSystemMemReq);
    if(!EventInitialize(&m_EventSystemMemReq, m_EventSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Event System");
        return false;
    }
    EventRegister(EVENT_CODE_APPLICATION_QUIT, this, Application::OnApplicationEventWrapper);
    EventRegister(EVENT_CODE_KEY_PRESSED, this, Application::ApplicationOnKeyWrapper);

    u64 m_InputSystemMemReq;
    InputInitialize(&m_InputSystemMemReq, 0);
    m_InputSystem = malloc(m_InputSystemMemReq);
    if(!InputInitialize(&m_InputSystemMemReq, m_InputSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Input System");
        return false;
    }

    u64 m_LoggerSystemMemReq;
    LoggerInitialize(&m_LoggerSystemMemReq, 0);
    m_LoggerSystem = malloc(m_LoggerSystemMemReq);
    if(!LoggerInitialize(&m_LoggerSystemMemReq, m_LoggerSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Logger System");
        return false;
    }

    if(!m_Renderer2D->Initialize())
    {
        BIT_LOG_ERROR("Failed To Initialze Renderer2D");
        return false;
    }
    if(!m_Renderer3D->Initialize())
    {
        BIT_LOG_ERROR("Failed To Initialze Renderer3D");
        return false;
    }

    u64 m_Physics2DSystemMemReq;
    BPhysics2DInitialize(&m_Physics2DSystemMemReq, 0);
    m_Physics2DSystem = malloc(m_Physics2DSystemMemReq);
    if(!BPhysics2DInitialize(&m_Physics2DSystemMemReq, m_Physics2DSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze BPhysics2D System");
        return false;
    }

    if(!m_GameInstance->OnInitialize({m_Renderer2D, m_Renderer3D, m_EntityManager, m_AssetManager, m_CameraManager}))
    {
        BIT_LOG_ERROR("Couldn't Initialize The Game..!");
        return false;
    }
    m_GameInstance->OnWindowResize( m_Width, m_Height);

    m_IsRunning = true;
    m_IsSuspended = false;
    return true;
}

void Application::Run()
{

    m_Time.Reset();
    while (m_IsRunning)
    {
        // if(!PlatformPumpMessages(&m_Platform))
        // {
        //     BIT_LOG_FATAL("Something Went Wrong with PlatformEventLoop");
        //     m_IsRunning = false;
        //     m_IsSuspended = true;
        // }
        PlatformPumpMessages(&m_Platform);
        if(!m_IsSuspended)
        {
            m_Time.Update();
            float deltaTime = m_Time.GetDeltaTime();

            if(!m_GameInstance->OnUpdate(deltaTime))
            {
                BIT_LOG_FATAL("Something Went Wrong with Game Update");
                m_IsRunning = false;
                break;
            }
            if(!m_GameInstance->OnRender())
            {
                BIT_LOG_FATAL("Something Went Wrong with Game Render");
                m_IsRunning = false;
                break;
            }
            m_EntityManager->Update();

            InputUpdate(deltaTime);
        }

    }
    EventUnRegister(EVENT_CODE_APPLICATION_QUIT, this, Application::OnApplicationEventWrapper);
    EventUnRegister(EVENT_CODE_KEY_PRESSED, this, Application::ApplicationOnKeyWrapper);
    EventShutdown(m_EventSystem);

    BPhysics2DShutdown(m_Physics2DSystem);
    LoggerShutdown(m_LoggerSystem);
    InputShutdown(m_InputSystem);
}

b8 Application::OnEvent(u16 code, EventContext data)
{
    switch(code) 
    {
        case EVENT_CODE_APPLICATION_QUIT:
        {
            BIT_LOG_DEBUG("App Is Quitting");
            m_IsRunning = false; 
            break;
        }
    }
    return true;
}
b8 Application::OnKey(u16 code, EventContext data)
{
    switch (code) 
    {
        case EVENT_CODE_KEY_PRESSED:
            {
                u16 key = data.U16[0];
                switch (key) 
                {
                    case KEY_ESCAPE:
                        {
                            EventContext zerodata = {};
                            EventFire(EVENT_CODE_APPLICATION_QUIT, 0, zerodata);
                        }
                }
                return true;
            }
    }
    return false;
}
b8 Application::OnApplicationEventWrapper(u16 code, void* sender, void* listener, EventContext data)
{
    Application* app = (Application*)listener;
    return app->OnEvent(code, data);
}
b8 Application::ApplicationOnKeyWrapper(u16 code, void* sender, void* listener, EventContext data)
{
    Application* app = (Application*)listener;
    return app->OnKey(code, data);
}
}
