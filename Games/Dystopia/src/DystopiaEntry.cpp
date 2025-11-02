#include "Bit/Core/Application.h"
#include "Dystopia.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    ApplicationConfig appConfig;
    Game* newGame = new Dystopia();
    newGame->m_AppConfig.x = 0;
    newGame->m_AppConfig.y = 0;
    newGame->m_AppConfig.width = 1280;
    newGame->m_AppConfig.height = 720;
    newGame->m_AppConfig.name = "Dystopia";
    return newGame;
}
