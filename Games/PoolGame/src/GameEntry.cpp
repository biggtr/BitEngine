#include "TestGame.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    Game* newGame = new TestGame;
    newGame->appConfig.y = 100;
    newGame->appConfig.width = 1366;
    newGame->appConfig.height = 768;
    newGame->appConfig.name = "TestGame";
    return newGame;
}
