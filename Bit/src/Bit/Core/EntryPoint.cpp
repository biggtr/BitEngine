#include "Bit/Bit.h"
extern BitEngine::Application* BitEngine::CreateApplication();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    BIT_CORE_INFO("Logger Initialized Correctly..!");
    BitEngine::EngineComponents* GameEngine = new BitEngine::EngineComponents();
    
    BitEngine::Window  window{};
    window.Create(SCR_WIDTH, SCR_HEIGHT,(char*)"Hello");
    GameEngine->Window = window;
    BIT_CORE_INFO("Window is Initialized.. From Engine!!..")
    auto app = BitEngine::CreateApplication();
    app->InitializeEngineSystems(GameEngine);
    app->Run();
    delete app;
    BIT_CORE_INFO("Im Gonna disappear for now..!");

    
    
    
    return 0;
}

