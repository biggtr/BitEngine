#include "Application.h"
#include <GLFW/glfw3.h>


namespace BitEngine
{

    
Application::Application()
{
    m_EngineComponents = nullptr;
}

void Application::InitializeEngineSystems(EngineComponents* engineComponents)
{
    m_EngineComponents = engineComponents;
    
}
void Application::OnInit()
{
    //default logic goes before OnInit
    const float vertices[] = 
    {
        -0.5,   0.5,   0.0, 1.0, 
        -0.5,  -0.5,   0.0, 0.0, 
        0.5,  -0.5,   1.0, 0.0, 
        0.5,   0.5,   1.0, 1.0, 
    };
    const float indices[] = 
    {
        0, 1, 2,
        0, 2, 3
    };
}
void Application::OnRender()
{
}
void Application::OnUpdate(float deltaTime)
{
}

void Application::Run()
{
    m_IsRunning = true;

    while(!m_EngineComponents->Window.ShouldClose() &&  m_IsRunning)
    {
        m_EngineComponents->Window.ProcessInput();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        OnUpdate(m_Time.GetDeltaTime());

        OnRender();

        m_EngineComponents->Window.SwapBuffers();

        glfwPollEvents();
    }
}




    
}
