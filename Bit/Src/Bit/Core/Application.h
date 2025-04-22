#pragma once
#include "Bit/Core/TimeStamp.h"
namespace BitEngine
{

        
class Application
{
public:


    Application();
    virtual ~Application();
    void Run();

private:
    void Init();
    void Update(float deltaTime);
    void Render();


protected:

    virtual void OnInit();
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();


private:
    bool m_IsRunning;
    Time m_Time;
    

};
Application* CreateApplication();

}
