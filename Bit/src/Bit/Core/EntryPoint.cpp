#include "Bit/Bit.h"
#include "Bit/Core/Application.h"
#include "Bit/Core/Game.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/RendererAPI.h"

int main()
{

    BitEngine::Game* game = BitEngine::CreateGame();

    BitEngine::Application* app = new BitEngine::Application();
   
    if(!app->Create(game))
    {
        BIT_LOG_ERROR("Couldnt create a new game");
        return -1;
    }

    app->Run();
    
    return 0;
}

