#include "Bit/Bit.h"
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
    }
    
};


BitEngine::Application* BitEngine::CreateApplication()
{
    return new Sandbox();
}
