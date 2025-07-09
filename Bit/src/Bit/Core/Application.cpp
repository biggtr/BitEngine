#include "Application.h"
#include "Bit/Core/Core.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Game.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Window.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Scene/Compontents.h"
#include <cstddef>


namespace BitEngine
{
Application Application::s_Instance;
bool Application::Create(Game *gameInstance)
{
    if(s_Instance.m_Initialized)
    {
        BIT_CORE_ERROR("You Cannot Create More Than One Application Instance At A Time");
        return false;
    }
    // Init All systems inhere 
    if(!s_Instance.Initialize(gameInstance->appConfig))
    {
        BIT_CORE_ERROR("Failed to initialize application");
        return false;
    }
    s_Instance.m_GameInstance = gameInstance;
    s_Instance.m_IsRunning = true;
    s_Instance.m_IsSuspended = false;

    if(!s_Instance.m_GameInstance->Initialize())
    {
        BIT_CORE_ERROR("Failed to initialize game instance");
        return false;
    }

    return true;
}

bool Application::Initialize(ApplicationConfig appCfg)
{
    m_Initialized = true;
    
    m_Window = BitEngine::Window::Create(appCfg.width, appCfg.height, appCfg.name);
    if(!m_Window)
    {
        BIT_CORE_ERROR("Failed to create window");
        return false;
    }
    
    m_Renderer2D = new BitEngine::Renderer2D();
    if(!m_Renderer2D)
    {
        BIT_CORE_ERROR("Failed to create renderer");
        return false;
    }
    return true;
}

void Application::Run()
{
    s_Instance.m_Time.Reset();

    while(s_Instance.m_IsRunning)
    {
        s_Instance.m_Time.Update();
        float deltaTime = s_Instance.m_Time.GetDeltaTime();
        s_Instance.m_Window->OnUpdate();

        if(!s_Instance.m_IsSuspended && s_Instance.m_GameInstance)
        {
            s_Instance.m_GameInstance->OnUpdate(deltaTime);

            // s_Instance.m_Renderer2D->BeginScene();
            s_Instance.m_Renderer2D->SetClearColor(BitMath::Vector4(0.0, 1.0, 0.0, 1.0));
            s_Instance.m_Renderer2D->Clear();
            s_Instance.m_GameInstance->OnRender();
            // s_Instance.m_Renderer2D->EndScene();
        }
    }
}
Application::~Application()
{
    delete m_Renderer2D;
    delete m_Window;
}

bool Application::Shutdown()
{
    s_Instance.m_IsRunning = false;
    return s_Instance.m_IsRunning;
}
}
