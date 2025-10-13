#include "Bit/Core/Application.h"
#include "PoolGame.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    ApplicationConfig appConfig;
    appConfig.x = 100;
    appConfig.y = 100;
    appConfig.width = 1366;
    appConfig.height = 768;
    appConfig.name = "TestGame";
    Game* newGame = new PoolGame();
    return newGame;
}
