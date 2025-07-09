#pragma once 
#include "Application.h"

namespace BitEngine 
{
extern Game* CreateGame();
class Game
{
public:
    ApplicationConfig appConfig;
public:
    Game(){}
    virtual ~Game(){}
    virtual bool Initialize() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
};
}
