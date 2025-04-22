#include "Application.h"


namespace BitEngine
{

    
Application::Application()
{
}

void Application::Init()
{
    //default logic goes before OnInit
    OnInit();
}
void Application::Render()
{
    //default logic goes before OnRender
    OnRender();
}
void Application::Update(float deltaTime)
{
    //default logic goes before OnUpdate 
    OnUpdate(deltaTime);
}

void Application::Run()
{
    while(m_IsRunning)
    {
        Update(m_Time.GetDeltaTime());
        Render();
    }
}



Application::~Application()
{
    
}

    
}
