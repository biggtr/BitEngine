#include "Bit.h"
#include <iostream>
class Sandbox : public BitEngine::Application

{
public:
    Sandbox()
    {
    }
    ~Sandbox()
    {

    }

    virtual void Run() override
    {
        std::cout << "SandBox....";
        while(true);
    }
    
};


BitEngine::Application* BitEngine::CreateApplication()
{
    return new Sandbox();
}
