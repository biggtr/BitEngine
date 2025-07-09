#include "Bit/Bit.h"
#include "Bit/Core/Application.h"
#include "Bit/Core/Game.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Window.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/RendererAPI.h"

int main()
{

    
    BitEngine::Game* game = BitEngine::CreateGame();

   
    if(!BitEngine::Application::Create(game))
    {
        BIT_CORE_ERROR("Couldnt create a new game");
        return -1;
    }

    BitEngine::Application::Run();

    if(BitEngine::Application::Shutdown())
    {
        delete game;
    }
    
    return 0;
}

