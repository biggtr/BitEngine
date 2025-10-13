#include "3DGame.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    Game* newGame = new Game3D();
    newGame->m_AppConfig.x = 0;
    newGame->m_AppConfig.y = 0;
    newGame->m_AppConfig.width = 1366;
    newGame->m_AppConfig.height = 768;
    newGame->m_AppConfig.name = "3dGame";
    return newGame;
}
