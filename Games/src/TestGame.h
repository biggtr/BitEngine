#pragma once 
#include <Bit/Core/Game.h>

class TestGame : public BitEngine::Game
{

public:
    TestGame(){}
    ~TestGame(){}
    virtual bool Initialize() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnRender() override;
};
