#include "Application.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Memory/ArenaAllocator.h"
#include "Bit/Particles/ParticleSystem.h"
#include "Bit/Physics/Physics2D.h"
#include "Bit/Renderer/GraphicsContext.h"
#include "Bit/Resources/AssetStore.h"
#include "Game.h"
#include "Platform/Platform.h"
#include <cstdlib>
#include <cstring>

namespace BitEngine
{

Application::Application()
{
    m_Renderer3D = new Renderer();
    m_Renderer2D = new Renderer2D();
    m_EntityManager = new EntityManager();
    m_CameraManager = new CameraManager();
    m_ParticleSystem = new ParticleSystem();
}

Application::~Application()
{
    delete m_ParticleSystem;
    delete m_CameraManager;
    delete m_EntityManager;
    delete m_Renderer2D;
    delete m_Renderer3D;
    delete m_GameInstance;
}

b8 Application::Create(Game* gameInstance)
{

    EventInitialize(&m_EventSystemMemReq, 0);
    InputInitialize(&m_InputSystemMemReq, 0);
    LoggerInitialize(&m_LoggerSystemMemReq, 0);
    Physics2DInitialize(&m_Physics2DSystemMemReq, 0);
    AssetsStoreInitialize(&m_AssetStoreSystemMemReq, 0);

    TotalSystemsMemorySize = m_Physics2DSystemMemReq + m_EventSystemMemReq + m_InputSystemMemReq + m_LoggerSystemMemReq + m_AssetStoreSystemMemReq;
    m_SystemsMemoryBlock = malloc(TotalSystemsMemorySize);
    if(!m_SystemsMemoryBlock)
    {
        BIT_LOG_ERROR("Failed To Allocate Systems Memory");
        return false;
    }
    memset(m_SystemsMemoryBlock, 0, TotalSystemsMemorySize);
    BIT_LOG_INFO("Allocated %llu bytes for all systems", TotalSystemsMemorySize);
    ArenaCreate(&m_SystemsArena, TotalSystemsMemorySize, m_SystemsMemoryBlock);

    m_EventSystem = ArenaAllocate(&m_SystemsArena, m_EventSystemMemReq);
    if(!EventInitialize(&m_EventSystemMemReq, m_EventSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Event System");
        return false;
    }
    EventRegister(EVENT_CODE_APPLICATION_QUIT, this, Application::OnApplicationEventWrapper);
    EventRegister(EVENT_CODE_KEY_PRESSED, this, Application::ApplicationOnKeyWrapper);

    m_InputSystem = ArenaAllocate(&m_SystemsArena, m_InputSystemMemReq); 
    if(!InputInitialize(&m_InputSystemMemReq, m_InputSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Input System");
        return false;
    }

    m_LoggerSystem = ArenaAllocate(&m_SystemsArena, m_LoggerSystemMemReq); 
    if(!LoggerInitialize(&m_LoggerSystemMemReq, m_LoggerSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Logger System");
        return false;
    }

    m_Physics2DSystem = ArenaAllocate(&m_SystemsArena, m_Physics2DSystemMemReq);
    if(!Physics2DInitialize(&m_Physics2DSystemMemReq, m_Physics2DSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Physics2D System");
        return false;
    }

    m_AssetStoreSystem = ArenaAllocate(&m_SystemsArena, m_AssetStoreSystemMemReq);
    if(!AssetsStoreInitialize(&m_AssetStoreSystemMemReq, m_AssetStoreSystem))
    {
        BIT_LOG_ERROR("Failed To Initialze Asset Store System");
        return false;
    }

    m_GameInstance = gameInstance;

    m_Width = m_GameInstance->m_AppConfig.width;
    m_Height = m_GameInstance->m_AppConfig.height;
    if(!PlatformStartup(&m_Platform,
                m_GameInstance->m_AppConfig.name,
                m_GameInstance->m_AppConfig.x,
                m_GameInstance->m_AppConfig.y,
                m_GameInstance->m_AppConfig.width,
                m_GameInstance->m_AppConfig.height
                ))
    {

        BIT_LOG_ERROR("Failed To Startup the platform");
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

    if(!m_GameInstance->OnInitialize({m_Renderer2D, m_Renderer3D, m_EntityManager, m_CameraManager, m_ParticleSystem}))
    {
        BIT_LOG_ERROR("Couldn't Initialize The Game..!");
        return false;
    }

    EventRegister(EVENT_CODE_WINDOW_RESIZED, this, Application::OnApplicationEventWrapper);
    PlatformPumpMessages(&m_Platform);

    m_IsRunning = true;
    m_IsSuspended = false;
    return true;
}

void Application::Run()
{
    m_Time.Reset();
    while (m_IsRunning)
    {
        if(!PlatformPumpMessages(&m_Platform))
        {
            BIT_LOG_FATAL("Something Went Wrong with PlatformEventLoop");
            m_IsRunning = false;
            m_IsSuspended = true;
        }
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
            m_Platform.Context->SwapBuffers();

            InputUpdate(deltaTime);
        }

    }
    PlatformShutdown(&m_Platform);

    AssetStoreShutdown(m_AssetStoreSystem);
    Physics2DShutdown(m_Physics2DSystem);
    LoggerShutdown(m_LoggerSystem);
    InputShutdown(m_InputSystem);

    EventUnRegister(EVENT_CODE_APPLICATION_QUIT, this, Application::OnApplicationEventWrapper);
    EventUnRegister(EVENT_CODE_WINDOW_RESIZED, this, Application::OnApplicationEventWrapper);
    EventUnRegister(EVENT_CODE_KEY_PRESSED, this, Application::ApplicationOnKeyWrapper);
    EventShutdown(m_EventSystem);

    if(m_SystemsMemoryBlock)
    {
        free(m_SystemsMemoryBlock);
        m_SystemsMemoryBlock = nullptr;
    }
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
        case EVENT_CODE_WINDOW_RESIZED:
        {
            u16 width = data.U16[0];
            u16 height = data.U16[1];
            BIT_LOG_DEBUG("Windows Resize -> width : %d, height : %d", width, height);
            m_GameInstance->OnWindowResize(width, height);
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
