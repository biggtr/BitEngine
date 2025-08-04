#pragma once 
#include <Bit/Core/Game.h>

class TestGame : public BitEngine::Game
{
private:

public:
    TestGame(){}
    ~TestGame(){}
    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
};
