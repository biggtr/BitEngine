#include "Application.h"
extern BitEngine::Application* BitEngine::CreateApplication();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    BitEngine::EngineComponents* GameEngine = new BitEngine::EngineComponents();

    BitEngine::Window  window{};
    window.Create(SCR_WIDTH, SCR_HEIGHT,(char*)"Hello");
    GameEngine->Window = window;

    auto app = BitEngine::CreateApplication();
    app->InitializeEngineSystems(GameEngine);
    app->Run();
    delete app;
    
    
    
    return 0;
}

