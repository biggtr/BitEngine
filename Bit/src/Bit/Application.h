#pragma once
namespace BitEngine
{

        
    class Application
    {
    public:
        Application()
        {
        
        }

        virtual ~Application()
        {

        }

        virtual void Run()
        {
            while(true);
        }
    };
    Application* CreateApplication();
}
