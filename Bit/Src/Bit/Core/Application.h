#pragma once
#include "Window.h"
#include "Bit/Core/TimeStamp.h"
namespace BitEngine
{

struct EngineComponents
{
    BitEngine::Window Window;

};
        
class Application
{
public:
    Application();
    virtual ~Application() = default;
    virtual void Run();
    void InitializeEngineSystems(EngineComponents* engineComponents);




protected:
    virtual void OnInit();
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();


private:
private:
    bool m_IsRunning;
    Time m_Time;
    EngineComponents* m_EngineComponents;
    

};
Application* CreateApplication();

}
