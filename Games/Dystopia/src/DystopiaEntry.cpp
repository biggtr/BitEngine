#include "Bit/Core/Application.h"
#include "Dystopia.h"
#include <Bit/Core/Game.h>


BitEngine::Game* BitEngine::CreateGame()
{
    ApplicationConfig appConfig;
    appConfig.x = 0;
    appConfig.y = 0;
    appConfig.width = 1080;
    appConfig.height = 1920;
    appConfig.name = "Dystopia";
    Game* newGame = new Dystopia();
    return newGame;
}
