#include "Application.h"
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Window.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Scene/Compontents.h"
#include <cstddef>


namespace BitEngine
{
    
Application::Application()
{
}
Application::~Application()
{
    delete m_EngineComponents.Window;
    delete m_EngineComponents.Renderer2D;
}
void Application::InitializeEngineSystems(const EngineComponents& engineComponents) 
{
    m_EngineComponents = engineComponents;
    m_EngineComponents.Renderer2D->Init();
    
}
void Application::OnInit()
{

    BIT_CORE_INFO("Window Width: {}", m_EngineComponents.Window->GetWidth());
    BIT_CORE_INFO("Window Height: {}", m_EngineComponents.Window->GetHeight());

    Archetype archetype;
    archetype.AddComponent<CHealth>({19.0f});
    archetype.AddComponent<CTest>({19.0f});
    auto signature = archetype.GetSignature();
    std::cout << signature << " This is a signature\n";
    
    BIT_CORE_INFO("ArcheType signature {}", signature);
    BIT_CORE_INFO("second Window Height: {}", m_EngineComponents.Window->GetHeight());

}
void Application::OnRender()
{
    m_EngineComponents.Renderer2D->SetClearColor(BitMath::Vector4(0.0f, 6.0f, 0.0f, 0.0f));
    m_EngineComponents.Renderer2D->Clear();
    m_EngineComponents.Renderer2D->DrawQuad(BitMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f));
}

void Application::OnUpdate(float deltaTime)
{
}

void Application::Run()
{
    m_IsRunning = true;
    OnInit();

    while(m_IsRunning)
    {
        OnUpdate(m_Time.GetDeltaTime());

        OnRender();

        m_EngineComponents.Window->OnUpdate();

    }
}



    
}
