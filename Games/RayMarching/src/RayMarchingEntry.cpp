#include "RayMarching.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    Game* newGame = new RayMarching();
    newGame->m_AppConfig.x = 0;
    newGame->m_AppConfig.y = 0;
    newGame->m_AppConfig.width = 1920;
    newGame->m_AppConfig.height = 1080;
    newGame->m_AppConfig.name = "RayMarching";
    return newGame;
}
