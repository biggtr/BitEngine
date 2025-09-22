#include "3DGame.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    Game* newGame = new Game3D();
    newGame->appConfig.x = 0;
    newGame->appConfig.y = 0;
    newGame->appConfig.width = 1366;
    newGame->appConfig.height = 768;
    newGame->appConfig.name = "3dGame";
    return newGame;
}
